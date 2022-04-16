//
// Created by shuo on 2022/4/16.
//

#ifndef TINYMUDUO_EVENTLOOP_H
#define TINYMUDUO_EVENTLOOP_H
#include <Base/Nocopyable.h>

namespace jing
{
    class EventLoop : public Nocopyable
    {
    public:
        void loop();
    };
}



#endif //TINYMUDUO_EVENTLOOP_H
