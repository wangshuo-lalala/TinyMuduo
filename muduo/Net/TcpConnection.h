//
// Created by shuo on 2022/5/4.
//

#ifndef TINYMUDUO_TCPCONNECTION_H
#define TINYMUDUO_TCPCONNECTION_H
#include <memory>
#include <Net/Buffer.h>
#include <Base/Nocopyable.h>
#include <Net/Socket.h>
#include <functional>
#include <Base/TimeStamp.h>
#include <Net/TypeDef.h>
namespace jing
{
    class TcpConnection;
    class EventLoop;
    class PollEventHandler;
    using TcpConnectionPtr = const std::shared_ptr<TcpConnection>&;
    using TcpCallBack = std::function<void(TcpConnectionPtr)>;
    class TcpConnection : public Nocopyable, public std::enable_shared_from_this<TcpConnection>
    {
    public:
        explicit TcpConnection(std::shared_ptr<EventLoop>, int socketFd);

        std::string readAll();

        void send(std::string text);

        void close();

        bool isclosed();

        void connectEstablished();

        void connectDestroyed();

        void handleRead();

        void handleWrite();

        void handleClose();

        void handleError();

        TimeStamp timeStamp() const {return m_timeStamp;}

        void setConnectionCallBack(TcpCallBack callBack)
        {
            m_connectionCallBack = std::move(callBack);
        }

        void setReadCallBack(TcpCallBack callBack)
        {
            m_readCallBack = std::move(callBack);
        }

        void setWriteCallBack(TcpCallBack callBack)
        {
            m_writeCallBack = std::move(callBack);
        }

        void setCloseCallBack(TcpCallBack callBack)
        {
            m_closeCallBack = std::move(callBack);
        }

        void setErrorCallBack(TcpCallBack callBack)
        {
            m_errorCallBack = std::move(callBack);
        }
    private:
        std::unique_ptr<Socket> m_socket;
        std::shared_ptr<EventLoop> m_loop;
        std::shared_ptr<PollEventHandler> m_handler;
        Buffer m_inputBuffer;
        Buffer m_outputBuffer;
        TimerID m_timeoutTimer;
        bool m_isClosed;
        TimeStamp m_timeStamp;
        TcpCallBack m_connectionCallBack;
        TcpCallBack m_readCallBack;
        TcpCallBack m_writeCallBack;
        TcpCallBack m_closeCallBack;
        TcpCallBack m_errorCallBack;
    };
}



#endif //TINYMUDUO_TCPCONNECTION_H
