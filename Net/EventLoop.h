//
// Created by shuo on 2022/4/16.
//

#ifndef TINYMUDUO_EVENTLOOP_H
#define TINYMUDUO_EVENTLOOP_H
#include <memory>
#include <list>
#include <thread>
#include <functional>
#include <mutex>
#include <Base/Nocopyable.h>
#include <Net/TypeDef.h>
#include <Base/TimeStamp.h>

namespace jing
{
    class Poller;
    class PollEventHandler;
    class TimerQueue;
    class EventLoop : public Nocopyable
    {
        using EventHanders = std::list<std::shared_ptr<PollEventHandler> >;
        using Func = std::vector<std::function<void()> >;
    public:
        EventLoop();
        ~EventLoop();

        void loop();

        void quit();

        void addHandler(std::shared_ptr<PollEventHandler> handler);

        void removeHandler(std::shared_ptr<PollEventHandler> handler);

        void updateHandler(std::shared_ptr<PollEventHandler> handler);

        bool hasHandler(std::shared_ptr<PollEventHandler> handler);

        bool isInloopThread();

        void assertInLoopThread();

        void runInloop(std::function<void()> func);

        void queueInloop(std::function<void()> func);

        void wakeUp();

        void callingFunctors();

        // run callback at time
        TimerID runAt(TimeStamp time, TimerCallBack callback);

        // run callback after delay(s)
        TimerID runAfter(double delay, TimerCallBack callback);

        // run callback every intervel
        TimerID runEvery(double intervel, TimerCallBack callback);

        void cancelTimer(TimerID timer);
    private:
        bool m_isLooping;
        bool m_callingFunctors;
        bool m_isHandling;

        const std::thread::id m_threadId;
        std::unique_ptr<Poller> m_poller;
        std::unique_ptr<TimerQueue> m_timerQueue;
        EventHanders m_activeHandlers;

        Func m_funcs;
        int m_wakeFd;
        std::unique_ptr<PollEventHandler> m_wakeHandler;
        std::mutex m_mutex;
    };
}



#endif //TINYMUDUO_EVENTLOOP_H
