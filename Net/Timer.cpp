//
// Created by shuo on 2022/4/22.
//

#include <Net/Timer.h>

jing::Timer::Timer(TimerCallBack callBack, jing::TimeStamp when, double interval)
:m_repeat(interval > 0.0),
 m_interval(interval),
 m_callBack(std::move(callBack)),
 m_expiration(when)
{

}

void jing::Timer::run()
{
    if (m_callBack)
    {
        m_callBack();
    }
}

jing::TimeStamp jing::Timer::expiration()
{
    return m_expiration;
}

bool jing::Timer::isRepeat() const
{
    return m_repeat;
}

void jing::Timer::restart(jing::TimeStamp now)
{
    if (m_repeat)
    {
        now.addTime(m_interval);
        m_expiration = now;
    }
    else
    {
        m_expiration = TimeStamp();
    }
}



