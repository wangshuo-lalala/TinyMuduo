//
// Created by shuo on 2022/5/2.
//

#include <Net/EventLoopThreadPool.h>
#include <Net/EventLoop.h>
#include <Net/EventLoopThread.h>
#include <Log/Logging.h>

jing::EventLoopThreadPool::EventLoopThreadPool(int threadNum)
:m_threadNum(threadNum),
m_isStarted(false)
{
    for (int i = 0; i < m_threadNum; ++i)
    {
        std::unique_ptr<EventLoopThread> thread = std::make_unique<EventLoopThread>();
        m_threads.emplace_back(std::move(thread));
    }
}

void jing::EventLoopThreadPool::start()
{
    if (m_isStarted)
    {
        LOG_WARN << "EventLoopThreadPool is started";
        return;
    }
    for (auto& thread : m_threads)
    {
        m_loops.emplace_back(std::move(thread->startLoop()));
    }
    m_isStarted = true;
}

std::shared_ptr<jing::EventLoop> jing::EventLoopThreadPool::getFitLoop()
{
    std::unique_lock<std::mutex> l(m_mutex);
    std::shared_ptr<EventLoop> res;
    int num = INT32_MAX;
    for (auto& loop : m_loops)
    {
        if (loop->getHandlerNum() < num)
        {
            num = loop->getHandlerNum();
            res = loop;
        }
    }
    return res;
}
