//
// Created by shuo on 2022/6/12.
//

#ifndef TINYMUDUO_EPOLLPOLLER_H
#define TINYMUDUO_EPOLLPOLLER_H

#include <Net/Poller.h>
#include <poll.h>
#include <sys/epoll.h>
namespace jing
{
    class EpollPoller : public Poller
    {
        using EpollEvents = std::vector<struct epoll_event>;
    public:
        EpollPoller();

        ~EpollPoller();

        void poll(int timeMs, EventHandlers& activeHandlers) override;

        // add  eventhandler to be monitored, inside the eventhandler will be changed to pollfd
        void addHandler(std::shared_ptr<PollEventHandler> handler) override;

        void removeHandler(std::shared_ptr<PollEventHandler> handler) override;

        void updateHandler(std::shared_ptr<PollEventHandler> handler) override;

        bool hasHandler(std::shared_ptr<PollEventHandler> handler) override;

        // in the poll() will test which handlers is active and push them into the activeHandlers;
        void fillActiveHandlers(int numEvents, EventHandlers& activeHandlers);
    private:
        int m_epollFd;
        EpollEvents m_epollEvents;
    };
}

#endif //TINYMUDUO_EPOLLPOLLER_H
