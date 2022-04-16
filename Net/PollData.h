//
// Created by shuo on 2022/4/16.
//

#ifndef TINYMUDUO_POLLDATA_H
#define TINYMUDUO_POLLDATA_H
#include <Base/Nocopyable.h>
#include <memory>


namespace jing
{
    class EventLoop;
    class EventCallBack;
    class PollData : public Nocopyable
    {
    public:
        void handleEvent();

        void setReadCallBack(std::shared_ptr<EventCallBack> callBack);
        void setWriteCallBack(std::shared_ptr<EventCallBack> callBack);
        void setCloseCallBack(std::shared_ptr<EventCallBack> callBack);
        void setErrorCallBack(std::shared_ptr<EventCallBack> callBack);

        void enableReading()
        {

        }
        void disableReading()
        {

        }
        void enableWriting()
        {

        }
        void disableWriting()
        {

        }
        void disableAll()
        {

        }
        bool isReading()
        {

        }
        bool isWriting()
        {

        }

    private:
        int m_fd;
        int m_events;
        int m_revents;

        std::shared_ptr<EventLoop> m_eventLoop;
        std::shared_ptr<EventCallBack> m_readCallBack;
        std::shared_ptr<EventCallBack> m_writeCallBack;
        std::shared_ptr<EventCallBack> m_closeCallBack;
        std::shared_ptr<EventCallBack> m_errorCallBack;
    };
}



#endif //TINYMUDUO_POLLDATA_H
