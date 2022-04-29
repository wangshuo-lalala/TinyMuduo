//
// Created by shuo on 2022/4/18.
//

#ifndef TINYMUDUO_POLLER_H
#define TINYMUDUO_POLLER_H
#include <Base/Nocopyable.h>
#include <Net/PollEventHandler.h>
#include <vector>
#include <memory>
#include <list>
#include <map>

namespace jing
{
    class Poller : public Nocopyable
    {
    protected:
        using EventHandlers = std::list<std::shared_ptr<PollEventHandler> >;
        using EventHandlerMap = std::map<int, std::shared_ptr<PollEventHandler> >;
    public:
        virtual void addHandler(std::shared_ptr<PollEventHandler> handler)= 0;

        virtual void removeHandler(std::shared_ptr<PollEventHandler> handler)= 0;

        virtual void updateHandler(std::shared_ptr<PollEventHandler> handler) = 0;

        virtual bool hasHandler(std::shared_ptr<PollEventHandler> handler) = 0;

        virtual void poll(int timeMs, EventHandlers& activeHandlers) = 0;

        static std::unique_ptr<Poller> createDefaultPoller();
    protected:
        EventHandlerMap m_handlerMap;
    };
}
#endif //TINYMUDUO_POLLER_H
