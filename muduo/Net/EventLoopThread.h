//
// Created by shuo on 2022/4/29.
//

#ifndef TINYMUDUO_EVENTLOOPTHREAD_H
#define TINYMUDUO_EVENTLOOPTHREAD_H
#include <Base/Nocopyable.h>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>
namespace jing
{
    class EventLoop;
    class EventLoopThread : public Nocopyable
    {
    public:
        EventLoopThread();
        ~EventLoopThread();

        std::shared_ptr<jing::EventLoop> startLoop();
    private:
        bool m_existing;
        std::thread m_thread;
        std::mutex m_mutex;
        std::condition_variable m_condition;
        std::shared_ptr<EventLoop> m_loop;
    };
}



#endif //TINYMUDUO_EVENTLOOPTHREAD_H
