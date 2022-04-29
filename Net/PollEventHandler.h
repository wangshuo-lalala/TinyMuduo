//
// Created by shuo on 2022/4/16.
//

#ifndef TINYMUDUO_POLLEVENTHANDLER_H
#define TINYMUDUO_POLLEVENTHANDLER_H
#include <Base/Nocopyable.h>
#include <Base/TimeStamp.h>
#include <memory>
#include <poll.h>
#include <functional>

namespace jing
{
    class EventLoop;
    class EventCallBack;
    class PollEventHandler : public Nocopyable
    {
        using EventCallBack = std::function<void()>;
    public:
        explicit PollEventHandler(int fd = -1);

        void handleEvent(TimeStamp stamp);

        void setReadCallBack(EventCallBack callBack);
        void setWriteCallBack(EventCallBack callBack);
        void setCloseCallBack(EventCallBack callBack);
        void setErrorCallBack(EventCallBack callBack);

        void enableReading()
        {
            m_events |= (POLLIN | POLLPRI);
        }
        void disableReading()
        {
            m_events &= ~(POLLIN | POLLPRI);
        }
        void enableWriting()
        {
            m_events |= POLLOUT;
        }
        void disableWriting()
        {
            m_events &= ~POLLOUT;
        }
        void disableAll()
        {
            m_events = 0;
        }
        bool isReading() const
        {
            return m_events & (POLLIN | POLLPRI);
        }
        bool isWriting() const
        {
            return m_events & POLLOUT;
        }

        int fd() const
        {
            return m_fd;
        }

        int events() const
        {
            return m_events;
        }

        int revents() const
        {
            return m_revents;
        }

        void setRevents(int events)
        {
            m_revents = events;
        }

        bool isHandling() const
        {
            return m_isHandling;
        }
    private:
        bool m_isHandling;

        int m_fd;
        int m_events;
        int m_revents;

        std::shared_ptr<EventLoop> m_eventLoop;
        EventCallBack m_readCallBack;
        EventCallBack m_writeCallBack;
        EventCallBack m_closeCallBack;
        EventCallBack m_errorCallBack;
    };
}



#endif //TINYMUDUO_POLLEVENTHANDLER_H
