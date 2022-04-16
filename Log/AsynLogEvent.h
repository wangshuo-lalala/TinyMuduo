//
// Created by shuo on 2022/4/14.
//

#ifndef MUDUO_ASYNLOGEVENT_H
#define MUDUO_ASYNLOGEVENT_H
#include <Log/ILogEvent.h>
#include <Log/FixBuffer.h>
#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>

namespace jing
{
    const int ROLL_SIZE = 1024 * 1024 * 1024;
    class AsynLogEvent : public ILogEvent
    {
        using BufferPtr = std::unique_ptr<Buffer>;
        using Buffers = std::vector<BufferPtr>;
    public:
        explicit AsynLogEvent(std::string fileName = "./jinglog", int rollSize = ROLL_SIZE);
        ~AsynLogEvent();
        void log(const char* buf, int length) override;

        void run();

    private:
        std::mutex m_mutex;
        std::condition_variable m_condition;
        std::thread m_thread;
        bool m_isRunning;
        int m_rollSize;
        int m_writenSize;
        int m_suffix;
        std::string m_fileName;
        BufferPtr m_currentBuffer;
        BufferPtr m_nextBuffer;
        Buffers m_buffers;
        Buffers m_buffersToWrite;
    };
}


#endif //MUDUO_ASYNLOGEVENT_H
