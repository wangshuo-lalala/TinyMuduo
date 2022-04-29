//
// Created by shuo on 2022/4/28.
//
#include <Net/Poller.h>
#include <Net/PollPoller.h>

std::unique_ptr<jing::Poller> jing::Poller::createDefaultPoller()
{
    if (::getenv("JING_USE_EPOLL"))
    {
        return nullptr;
    }
    else
    {
        return std::make_unique<PollPoller>();
    }

}
