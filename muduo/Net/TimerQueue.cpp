//
// Created by shuo on 2022/4/23.
//
#include <Net/TimerQueue.h>
#include <Net/EventLoop.h>
#include <Base/Utils.h>
#include <Net/PollEventHandler.h>
jing::TimerQueue::TimerQueue(EventLoop *loop)
:m_loop(loop),
 m_timerFd(util::createTimerFd()),
 m_timerEventHandler(std::make_shared<PollEventHandler>(m_timerFd))
{
    m_timerEventHandler->setReadCallBack([this]()
                                         {
                                            handleRead();
                                         });
    m_timerEventHandler->enableReading();
    loop->addHandler(m_timerEventHandler);
}

TimerID jing::TimerQueue::addTimer(std::shared_ptr<Timer> timer)
{
    //m_loop->assertInLoopThread();
    auto res = reinterpret_cast<TimerID>(timer.get());
    m_loop->runInloop([=]()
                      {
                          m_loop->assertInLoopThread();
                          m_timerSet.insert(timer);
                          updateTimer();
                      });
    return res;
}

void jing::TimerQueue::cancelTimer(TimerID timerId)
{
    m_loop->assertInLoopThread();
    for (auto& timer : m_timerSet)
    {
        auto tId = reinterpret_cast<TimerID>(timer.get());
        if (tId == timerId)
        {
            m_timerSet.erase(timer);
            break;
        }
    }
}

void jing::TimerQueue::handleRead()
{
    m_loop->assertInLoopThread();
    util::readTimerFd(m_timerFd, TimeStamp::now());
    auto itr = m_timerSet.begin();
    for (; itr != m_timerSet.end(); ++itr)
    {
        if ((*itr)->expiration() < TimeStamp::now())
        {
            (*itr)->run();
            m_expired.insert(*itr);
        }
        else
        {
            break;
        }
    }
    m_timerSet.erase(m_timerSet.begin(), itr);
    for (auto& timer : m_expired)
    {
        if (timer->isRepeat())
        {
            timer->restart(TimeStamp::now());
            m_timerSet.insert(timer);
        }
    }
    m_expired.clear();
    updateTimer();
}

void jing::TimerQueue::updateTimer()
{
    m_loop->assertInLoopThread();
    if (!m_timerSet.empty())
    {
        auto itr = m_timerSet.begin();
        util::resetTimerFd(m_timerFd, (*itr)->expiration());
    }
}

