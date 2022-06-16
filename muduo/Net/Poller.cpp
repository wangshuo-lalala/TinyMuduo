//
// Created by shuo on 2022/4/28.
//
#include <Net/Poller.h>
#include <Net/PollPoller.h>
#include <Net/EpollPoller.h>
std::unique_ptr<jing::Poller> jing::Poller::createDefaultPoller()
{
    if (::getenv("JING_USE_EPOLL"))
    {
        return nullptr;
    }
    else
    {
        return std::make_unique<EpollPoller>();
    }

}

int jing::Poller::getHandlerNum() const
{
    return static_cast<int>(m_handlerMap.size());
}
