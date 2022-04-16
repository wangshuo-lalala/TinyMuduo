//
// Created by shuo on 2022/4/14.
//

#ifndef MUDUO_DATE_H
#define MUDUO_DATE_H
#include <string>
#include <chrono>
using namespace std::chrono;
namespace jing
{
    std::string now();

    class TimeCount
    {
    public:
        TimeCount();
        ~TimeCount();

    private:
        time_point<system_clock> m_start;
        time_point<system_clock> m_end;
    };
}


#endif //MUDUO_DATE_H
