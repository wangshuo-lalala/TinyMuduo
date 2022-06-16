//
// Created by shuo on 2022/6/12.
//

#ifndef TINYMUDUO_TCPCLIENT_H
#define TINYMUDUO_TCPCLIENT_H
#include <Base/Nocopyable.h>
#include <Net/InetAddress.h>
#include <Net/TypeDef.h>
#include <Net/Socket.h>
#include <Net/TcpConnection.h>
#include <memory>
#include <map>
namespace jing
{
    class EventLoop;
    class TcpClient : public Nocopyable
    {
    public:
        explicit TcpClient();
        ~TcpClient();

        void connectTo(const InetAddress& address);

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
        bool m_isConnecting;
        std::shared_ptr<EventLoop> m_mainLoop;
        std::shared_ptr<TcpConnection> m_connection;
        Socket m_socket;
        TcpCallBack m_connectionCallBack;
        TcpCallBack m_readCallBack;
        TcpCallBack m_writeCallBack;
        TcpCallBack m_closeCallBack;
        TcpCallBack m_errorCallBack;
    };
}



#endif //TINYMUDUO_TCPCLIENT_H
