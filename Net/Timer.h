//
// Created by shuo on 2022/4/22.
//

#ifndef TINYMUDUO_TIMER_H
#define TINYMUDUO_TIMER_H

#include <Net/TypeDef.h>
#include <Base/Nocopyable.h>
#include <Base/TimeStamp.h>
namespace jing
{
    class Timer : public Nocopyable
    {
    public:
        explicit Timer(TimerCallBack callBack, TimeStamp when, double interval = 0.0);
        ~Timer() = default;

        void run();

        TimeStamp expiration();

        bool isRepeat() const;

        void restart(TimeStamp now);
    private:
        TimeStamp m_expiration;
        TimerCallBack m_callBack;
        const bool m_repeat;
        const double m_interval;
    };
}



#endif //TINYMUDUO_TIMER_H
