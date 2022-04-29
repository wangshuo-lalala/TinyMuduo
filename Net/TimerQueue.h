//
// Created by shuo on 2022/4/23.
//

#ifndef TINYMUDUO_TIMERQUEUE_H
#define TINYMUDUO_TIMERQUEUE_H
#include <Base/Nocopyable.h>
#include <Net/TypeDef.h>
#include <Net/Timer.h>
#include <memory>
#include <set>
#include <map>


namespace jing
{
    class EventLoop;
    class PollEventHandler;
    class TimerQueue : public Nocopyable
    {
        class Compare
        {
        public:
            bool operator()(const std::shared_ptr<Timer>& timer1, const std::shared_ptr<Timer>& timer2) const
            {
                return timer1->expiration().microSecondSinceEpoch() <= timer2->expiration().microSecondSinceEpoch();
            }
        };
        using TimerSet = std::set<std::shared_ptr<Timer>, Compare>;

    public:
        explicit TimerQueue(EventLoop *loop);
        ~TimerQueue() = default;


        TimerID addTimer(std::shared_ptr<Timer> timer);

        // pass the id from addTimer
        void cancelTimer(TimerID timerId);

        void  handleRead();

        void updateTimer();
    private:
        EventLoop* m_loop;
        TimerSet m_timerSet;
        TimerSet m_expired;
        const int m_timerFd;
        std::shared_ptr<PollEventHandler> m_timerEventHandler;
    };
}



#endif //TINYMUDUO_TIMERQUEUE_H
