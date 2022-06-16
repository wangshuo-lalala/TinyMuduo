//
// Created by shuo on 2022/4/13.
//

#ifndef MUDUO_ILOGEVENT_H
#define MUDUO_ILOGEVENT_H
namespace jing {
    class ILogEvent
    {
    public:
        virtual void log(const char* buf, int length) = 0;
    };
}
#endif //MUDUO_ILOGEVENT_H
