//
// Created by shuo on 2022/4/29.
//

#include <Net/EventLoopThread.h>
#include <Net/EventLoop.h>


jing::EventLoopThread::EventLoopThread()
:m_existing(false),
 m_loop(nullptr)
{

}

jing::EventLoopThread::~EventLoopThread()
{
    if (m_loop)
    {
        m_loop->quit();
        m_thread.join();
    }
}
std::shared_ptr<jing::EventLoop> jing::EventLoopThread::startLoop()
{
    if (m_existing)
    {
        return m_loop;
    }
    m_existing = true;
    m_thread = std::move(std::thread([=]()
                                     {
                                         {
                                             std::unique_lock l(m_mutex);
                                             m_loop = std::make_shared<EventLoop>();
                                             m_condition.notify_one();
                                         }
                                         m_loop->loop();

                                     }));
    {
        std::unique_lock l(m_mutex);
        while (!m_loop)
        {
            m_condition.wait(l);
        }
        return m_loop;
    }
}
