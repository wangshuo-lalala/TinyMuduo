//
// Created by shuo on 2022/4/14.
//

#include <Log/AsynLogEvent.h>
#include <stdio.h>

jing::AsynLogEvent::AsynLogEvent(std::string fileName, int rollSize)
        :m_fileName{std::move(fileName)},
         m_rollSize{rollSize},
         m_isRunning(false),
         m_writenSize(0),
         m_suffix(0)
{
    m_isRunning = true;
    m_thread = std::thread([this]()
                           {
                               run();
                           });
}

jing::AsynLogEvent::~AsynLogEvent()
{
    m_isRunning = false;
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void jing::AsynLogEvent::log(const char* buf, int length)
{
    std::unique_lock<std::mutex> l(m_mutex);
    if (!m_currentBuffer->append(buf, length))
    {
        m_buffers.emplace_back(std::move(m_currentBuffer));
        if (m_nextBuffer)
        {
            m_currentBuffer = std::move(m_nextBuffer);
        }
        else
        {
            m_currentBuffer = std::make_unique<Buffer>();
        }
        m_currentBuffer->append(buf, length);
        m_condition.notify_one();
    }
}

void jing::AsynLogEvent::run()
{
    m_currentBuffer = std::make_unique<Buffer>();
    m_nextBuffer = std::make_unique<Buffer>();
    BufferPtr bufferCache1 = std::make_unique<Buffer>();
    BufferPtr bufferCache2 = std::make_unique<Buffer>();
    FILE* pFile = fopen(m_fileName.c_str(), "a");
    if (!pFile)
    {
        fputs("fileName is not valid", stderr);
        m_isRunning = false;
    }
    while (m_isRunning)
    {
        {
            std::unique_lock<std::mutex> l(m_mutex);
            if (m_buffers.empty())
            {
                m_condition.wait_for(l, std::chrono::milliseconds(3));
            }
            m_buffers.emplace_back(std::move(m_currentBuffer));
            m_currentBuffer = std::move(bufferCache1);
            if (!m_nextBuffer)
            {
                m_nextBuffer = std::move(bufferCache2);
            }
            m_buffersToWrite.swap(m_buffers);
        }
        for (auto& buffer: m_buffersToWrite)
        {
            if (buffer->length() > 0)
            {
                m_writenSize += buffer->length();
                if (m_writenSize > m_rollSize)
                {
                    m_writenSize = buffer->length();
                    fclose(pFile);
                    std::string name = m_fileName + std::to_string(m_suffix++);
                    pFile = fopen(name.c_str(), "a");
                    if (!pFile)
                    {
                        fputs("fileName is not valid", stderr);
                        m_isRunning = false;
                    }
                }
                fwrite(buffer->data(), sizeof(char), buffer->length(), pFile);
            }
            buffer->clear();
        }
        if (m_buffersToWrite.size() > 2)
        {
            m_buffersToWrite.resize(2);
        }

        if (!bufferCache1)
        {
            bufferCache1 = std::move(m_buffersToWrite.back());
            m_buffersToWrite.pop_back();
        }

        if (!bufferCache2)
        {
            bufferCache2 = std::move(m_buffersToWrite.back());
            m_buffersToWrite.pop_back();
        }

        m_buffersToWrite.clear();
    }
    fclose(pFile);
}

