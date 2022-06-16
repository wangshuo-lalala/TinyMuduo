//
// Created by shuo on 2022/4/16.
//

#include <Net/EventLoop.h>
#include <Net/TimerQueue.h>
#include <Net/Timer.h>
#include <Net/Poller.h>
#include <Base/Utils.h>
#include <utility>
#include <Log/Logging.h>
#include <cassert>

// one loop per thread
__thread jing::EventLoop* loopInThread = nullptr;

jing::EventLoop::EventLoop()
:m_poller(std::move(Poller::createDefaultPoller())),
 m_isLooping(false),
 m_isHandling(false),
 m_wakeFd(util::createEventFd()),
 m_wakeHandler(std::make_unique<PollEventHandler>(m_wakeFd)),
 m_threadId(std::this_thread::get_id()),
 m_timerQueue(std::make_unique<TimerQueue>(this))
{
    if (loopInThread)
    {
        LOG_ERROR << "another thread " << loopInThread << "in this thread\n";
    }
    else
    {
        loopInThread = this;
    }
    m_wakeHandler->setReadCallBack([this]()
                                   {
                                        uint64_t one;
                                        auto n = ::read(m_wakeFd, &one, sizeof one);
                                        if (n < 0)
                                        {
                                            LOG_ERROR << "eventFd read error\n";
                                        }
                                   });
    m_wakeHandler->enableReading();
}

void jing::EventLoop::loop()
{
    assertInLoopThread();
    m_isLooping = true;
    while (m_isLooping)
    {
        m_activeHandlers.clear();
        m_poller->poll(1000, m_activeHandlers); // TODO
        m_isHandling = true;
        for (const auto& handler : m_activeHandlers)
        {
            handler->handleEvent(TimeStamp()); //TODO
        }
        m_isHandling = false;
        callingFunctors();
    }
}

void jing::EventLoop::addHandler(std::shared_ptr<PollEventHandler> handler)
{
    if (m_poller)
    {
        m_poller->addHandler(std::move(handler));
    }
}

void jing::EventLoop::removeHandler(std::shared_ptr<PollEventHandler> handler)
{
    if (m_poller)
    {
        m_poller->removeHandler(std::move(handler));
    }
}

void jing::EventLoop::updateHandler(std::shared_ptr<PollEventHandler> handler)
{
    if (m_poller)
    {
        m_poller->updateHandler(std::move(handler));
    }
}

bool jing::EventLoop::hasHandler(std::shared_ptr<PollEventHandler> handler)
{
    if (!m_poller)
    {
        return false;
    }
    else
    {
        return m_poller->hasHandler(std::move(handler));
    }
}

void jing::EventLoop::quit()
{
    m_isLooping = false;
}

jing::EventLoop::~EventLoop()
{
    loopInThread = nullptr;
}

void jing::EventLoop::assertInLoopThread()
{
    assert(m_threadId == std::this_thread::get_id());
}

bool jing::EventLoop::isInloopThread()
{
    return m_threadId == std::this_thread::get_id();
}

void jing::EventLoop::runInloop(std::function<void()> func)
{
    if (isInloopThread() && m_isLooping)
    {
        func();
    }
    else
    {
        queueInloop(std::move(func));
    }
}

void jing::EventLoop::queueInloop(std::function<void()> func)
{
    std::unique_lock l(m_mutex);
    m_funcs.emplace_back(std::move(func));
    if (!isInloopThread() || m_callingFunctors)
    {
        wakeUp();
    }
}

void jing::EventLoop::callingFunctors()
{
    m_callingFunctors = true;
    Func funcs;
    {
        std::unique_lock l(m_mutex);
        funcs.swap(m_funcs);
    }
    for (const auto& func : funcs)
    {
        func();
    }
    m_callingFunctors = false;
}

TimerID jing::EventLoop::runAt(jing::TimeStamp time, TimerCallBack callback)
{
    std::shared_ptr<Timer> timer = std::make_shared<Timer>(std::move(callback), time);
    return m_timerQueue->addTimer(std::move(timer));
}

TimerID jing::EventLoop::runAfter(double delay, TimerCallBack callback)
{
    TimeStamp now = TimeStamp::now();
    now.addTime(delay);
    std::shared_ptr<Timer> timer = std::make_shared<Timer>(std::move(callback), now);
    return m_timerQueue->addTimer(std::move(timer));
}

TimerID jing::EventLoop::runEvery(double intervel, TimerCallBack callback)
{
    TimeStamp now = TimeStamp::now();
    std::shared_ptr<Timer> timer = std::make_shared<Timer>(std::move(callback), now, intervel);
    return m_timerQueue->addTimer(std::move(timer));
}

void jing::EventLoop::cancelTimer(TimerID timer)
{
    m_timerQueue->cancelTimer(timer);
}

void jing::EventLoop::wakeUp()
{
    uint64_t one = 1;
    auto n = ::write(m_wakeFd,&one,sizeof one);
    if (n < sizeof one)
    {
        LOG_ERROR << "eventfd write error";
    }
}

int jing::EventLoop::getHandlerNum() const
{
    return m_poller->getHandlerNum();
}


