//
// Created by shuo on 2022/5/2.
//

#ifndef TINYMUDUO_EVENTLOOPTHREADPOOL_H
#define TINYMUDUO_EVENTLOOPTHREADPOOL_H
#include <Base/Nocopyable.h>
#include <vector>
#include <memory>
#include <mutex>

namespace jing
{
    class EventLoopThread;
    class EventLoop;
    class EventLoopThreadPool
    {
        using Loops = std::vector<std::shared_ptr<EventLoop> >;
        using Threads = std::vector<std::unique_ptr<EventLoopThread>>;
    public:
        explicit EventLoopThreadPool(int threadNum = 1);
        ~EventLoopThreadPool() =default;

        void start();

        std::shared_ptr<EventLoop> getFitLoop();
    private:
        int m_threadNum;
        bool m_isStarted;
        Loops m_loops;
        Threads m_threads;
        std::mutex m_mutex;
    };
}



#endif //TINYMUDUO_EVENTLOOPTHREADPOOL_H
