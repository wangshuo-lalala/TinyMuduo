//
// Created by shuo on 2022/6/12.
//
#include <Net/EpollPoller.h>
#include <Log/Logging.h>
#include <unistd.h>
static const int initialSize = 16;
jing::EpollPoller::EpollPoller()
:m_epollFd(epoll_create1(EPOLL_CLOEXEC)),
m_epollEvents(initialSize)
{
    if (m_epollFd < 0)
    {
        LOG_ERROR << "epoll create falid";
    }
}

void jing::EpollPoller::poll(int timeMs, jing::Poller::EventHandlers &activeHandlers)
{
    int numEvents = ::epoll_wait(m_epollFd, m_epollEvents.data(), static_cast<int>(m_epollEvents.size()), timeMs);
    if (numEvents > 0)
    {
        fillActiveHandlers(numEvents, activeHandlers);
    }
}

jing::EpollPoller::~EpollPoller()
{
    ::close(m_epollFd);
}

void jing::EpollPoller::fillActiveHandlers(int numEvents, jing::Poller::EventHandlers &activeHandlers)
{
    for (int i = 0; i < numEvents; ++i)
    {
        auto handler = m_handlerMap[m_epollEvents[i].data.fd];
        handler->setRevents(m_epollEvents[i].events);
        activeHandlers.emplace_back(std::move(handler));
    }
}

void jing::EpollPoller::addHandler(std::shared_ptr<PollEventHandler> handler)
{
    if (hasHandler(handler))
    {
        LOG_WARN << "the handler with the fd=" << handler->fd() << "is added!\n";
        return;
    }
    struct epoll_event event;
    memset(&event, 0, sizeof event);
    event.events = handler->events();
    event.data.fd = handler->fd();
    if (::epoll_ctl(m_epollFd, EPOLL_CTL_ADD, handler->fd(), &event) < 0)
    {
        LOG_ERROR << "epoll ctl add failed";
    }
    m_handlerMap[handler->fd()] = std::move(handler);
}

void jing::EpollPoller::removeHandler(std::shared_ptr<PollEventHandler> handler)
{
    if (!hasHandler(handler))
    {
        LOG_WARN << "the handler with the fd=" << handler->fd() << "is not removed!\n";
        return;
    }
    struct epoll_event event;
    memset(&event, 0, sizeof event);
    event.events = handler->events();
    event.data.fd = handler->fd();
    if (::epoll_ctl(m_epollFd, EPOLL_CTL_DEL, handler->fd(), &event) < 0)
    {
        LOG_ERROR << "epoll ctl add failed";
    }
    m_handlerMap.erase(m_handlerMap.find(handler->fd()));
}

bool jing::EpollPoller::hasHandler(std::shared_ptr<PollEventHandler> handler)
{
    if (m_handlerMap.find(handler->fd()) != m_handlerMap.end())
    {
        return true;
    }
    return false;
}

void jing::EpollPoller::updateHandler(std::shared_ptr<PollEventHandler> handler)
{
    if (!hasHandler(handler))
    {
        addHandler(handler);
    }
    removeHandler(m_handlerMap[handler->fd()]);
    addHandler(handler);
}


