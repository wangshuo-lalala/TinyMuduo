//
// Created by shuo on 2022/4/16.
//

#ifndef TINYMUDUO_TIMESTAMP_H
#define TINYMUDUO_TIMESTAMP_H
#include <chrono>
using namespace  std::chrono;
namespace jing
{
    class TimeStamp
    {
    public:
        explicit TimeStamp(int64_t microSecondSinceEpoch = 0)
        :m_microSecondSinceEpoch(microSecondSinceEpoch)
        {

        }

        int64_t microSecondSinceEpoch()
        {
            return m_microSecondSinceEpoch;
        }
        time_t secondSinceEpoch()
        {
            return static_cast<time_t>(m_microSecondSinceEpoch / 1000000);
        }

        void addTime(double seconds)
        {
            m_microSecondSinceEpoch += static_cast<int64_t>(seconds) * 1000000;
        }

        bool operator<(TimeStamp rhs) const
        {
            return this->m_microSecondSinceEpoch < rhs.m_microSecondSinceEpoch;
        }

        void updateToNow()
        {
            auto timeNow = system_clock::now();
            m_microSecondSinceEpoch = duration_cast<microseconds>(timeNow.time_since_epoch()).count();
        }

        static TimeStamp now()
        {
            auto timeNow = system_clock::now();
            auto timeSinceEpoch = duration_cast<microseconds>(timeNow.time_since_epoch()).count();
            return TimeStamp(timeSinceEpoch);
        }
    private:
        int64_t m_microSecondSinceEpoch;
    };
}

#endif //TINYMUDUO_TIMESTAMP_H
