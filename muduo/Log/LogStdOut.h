//
// Created by shuo on 2022/4/13.
//

#ifndef MUDUO_LOGSTDOUT_H
#define MUDUO_LOGSTDOUT_H

#include <Log/ILogEvent.h>
#include <cstdio>
namespace jing
{
    class LogStdOut : public jing::ILogEvent
    {
    public:
        void log(const char* buf, int length) override
        {
            fwrite(buf, sizeof(char), length, stdout);
        }
    };
}
#endif //MUDUO_LOGSTDOUT_H
