//
// Created by shuo on 2022/4/14.
//

#include <Log/Date.h>
#include <iostream>

namespace jing
{
    std::string now()
    {
        time_t timeNow = time(nullptr);
        tm* localTime = localtime(&timeNow);
        char buf[1024];
        strftime(buf, 1024, "%Y-%m-%d %H:%M:%S ", localTime);
        std::string res(buf);
        return res;
    }

    TimeCount::TimeCount()
    {
        m_start = system_clock::now();
    }

    TimeCount::~TimeCount()
    {
        m_end = system_clock::now();
        auto duration = m_end - m_start;
        std::cout << "you spend " << duration_cast<seconds>(duration).count() << "s" << std::endl;
    }
}
