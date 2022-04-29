//
// Created by shuo on 2022/4/26.
//

#ifndef TINYMUDUO_UTILS_H
#define TINYMUDUO_UTILS_H
#include <sys/timerfd.h>
#include <sys/eventfd.h>
#include <Log/Logging.h>
#include <Base/TimeStamp.h>
#include <unistd.h>

namespace jing
{
    namespace util
    {
        static int createEventFd()
        {
            int eventFd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
            if (eventFd < 0)
            {
                LOG_ERROR << "create eventfd failed";
            }
            return eventFd;
        }

        static int createTimerFd()
        {
            int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
            if (timerfd < 0)
            {
                LOG_ERROR << "create timerfd failed";
            }
            return timerfd;
        }

        static void readTimerFd(int timerFd, TimeStamp now)
        {
            uint64_t howmany;
            auto n = ::read(timerFd, &howmany, sizeof howmany);
            if (n < sizeof howmany)
            {
                LOG_ERROR << "timer read error\n";
            }
        }

        static void resetTimerFd(int timerfd, TimeStamp when)
        {
            struct itimerspec howLong;
            bzero(&howLong, sizeof howLong);
            auto duration = when.microSecondSinceEpoch() - TimeStamp::now().microSecondSinceEpoch();
            if (duration < 100)
            {
                duration = 100;
            }

            howLong.it_value.tv_sec = duration / 1000000;
            howLong.it_value.tv_nsec = duration % 1000000 * 1000;
            ::timerfd_settime(timerfd, 0, &howLong, nullptr);
        }
    }
}
#endif //TINYMUDUO_UTILS_H
