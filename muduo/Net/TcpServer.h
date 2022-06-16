//
// Created by shuo on 2022/5/2.
//

#ifndef TINYMUDUO_TCPSERVER_H
#define TINYMUDUO_TCPSERVER_H
#include <Base/Nocopyable.h>
#include <Net/InetAddress.h>
#include <Net/TypeDef.h>
#include <Net/Socket.h>
#include <Net/TcpConnection.h>
#include <memory>
#include <map>
namespace jing
{
    class EventLoopThreadPool;
    class EventLoop;
    using ConnectionMap = std::map<int, std::shared_ptr<TcpConnection> >;
    class TcpServer : public Nocopyable
    {
    public:
        explicit TcpServer(int threadNum = 1);
        ~TcpServer();

        void listen(const InetAddress& address);

        bool isListening() const;

        void close();

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
        void newConnection();
    private:
        bool m_isListening;
        std::unique_ptr<EventLoopThreadPool> m_loopPool;
        std::unique_ptr<EventLoop> m_mainLoop;
        ConnectionMap m_connectionMap;
        Socket m_listenSocket;
        TcpCallBack m_connectionCallBack;
        TcpCallBack m_readCallBack;
        TcpCallBack m_writeCallBack;
        TcpCallBack m_closeCallBack;
        TcpCallBack m_errorCallBack;
    };
}



#endif //TINYMUDUO_TCPSERVER_H
