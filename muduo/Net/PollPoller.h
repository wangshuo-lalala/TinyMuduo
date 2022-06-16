//
// Created by shuo on 2022/4/19.
//

#ifndef TINYMUDUO_POLLPOLLER_H
#define TINYMUDUO_POLLPOLLER_H
#include <Base/Nocopyable.h>
#include <Net/Poller.h>
#include <map>
#include <list>
#include <poll.h>

namespace jing
{
    class PollPoller : public Poller
    {
        using PollFds = std::vector<struct pollfd>;
    public:
        void poll(int timeMs, EventHandlers& activeHandlers) override;

        // add  eventhandler to be monitored, inside the eventhandler will be changed to pollfd
        void addHandler(std::shared_ptr<PollEventHandler> handler) override;

        void removeHandler(std::shared_ptr<PollEventHandler> handler) override;

        void updateHandler(std::shared_ptr<PollEventHandler> handler) override;

        bool hasHandler(std::shared_ptr<PollEventHandler> handler) override;

        // in the poll() will test which handlers is active and push them into the activeHandlers;
        void fillActiveHandlers(EventHandlers& activeHandlers);
    private:
        PollFds m_pollFds;
    };
}



#endif //TINYMUDUO_POLLPOLLER_H
