//
// Created by shuo on 2022/4/19.
//

#include <Net/PollPoller.h>
#include <Log/Logging.h>
#include <Base/TimeStamp.h>

void jing::PollPoller::poll(int timeMs, jing::PollPoller::EventHandlers &activeHandlers)
{
    int numEvents = ::poll(m_pollFds.data(), m_pollFds.size(), timeMs);
    TimeStamp now(TimeStamp::now());
    if (numEvents > 0)
    {
        fillActiveHandlers(activeHandlers);
    }
}

void jing::PollPoller::addHandler(std::shared_ptr<PollEventHandler> handler)
{
    if (hasHandler(handler))
    {
        LOG_WARN << "the handler with the fd=" << handler->fd() << "is added!\n";
        return;
    }
    if (handler->fd() < 0)
    {
        LOG_WARN << "the handler with the fd=" << handler->fd() << "is invalid!\n";
        return;
    }
    else
    {
        struct pollfd pfd{};
        pfd.fd = handler->fd();
        pfd.events = static_cast<short>(handler->events());
        pfd.revents = 0;
        m_pollFds.emplace_back(pfd);
        m_handlerMap[handler->fd()] = std::move(handler);
    }
}

void jing::PollPoller::removeHandler(std::shared_ptr<PollEventHandler> handler)
{
    if (!hasHandler(handler))
    {
        LOG_WARN << "the handler with the fd=" << handler->fd() << "is not added!\n";
        return;
    }
    else
    {
        m_handlerMap.erase(m_handlerMap.find(handler->fd()));
    }
}

void jing::PollPoller::updateHandler(std::shared_ptr<PollEventHandler> handler)
{
    if (!hasHandler(handler))
    {
        addHandler(handler);
    }
    else
    {
        for (auto& pollfd : m_pollFds)
        {
            if (pollfd.fd == handler->fd())
            {
                pollfd.fd = handler->fd();
                pollfd.events = static_cast<short>(handler->events());
                pollfd.revents = 0;
            }
        }
    }
}

bool jing::PollPoller::hasHandler(std::shared_ptr<PollEventHandler> handler)
{
    if (m_handlerMap.find(handler->fd()) != m_handlerMap.end())
    {
        return true;
    }
    return false;
}

void jing::PollPoller::fillActiveHandlers(jing::PollPoller::EventHandlers &activeHandlers)
{
    for (auto pollfd : m_pollFds)
    {
        if (pollfd.revents > 0)
        {
            m_handlerMap[pollfd.fd]->setRevents(pollfd.revents);
            activeHandlers.emplace_back(m_handlerMap[pollfd.fd]);
        }
    }
}






