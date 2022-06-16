//
// Created by shuo on 2022/6/12.
//

#include <Net/TcpClient.h>
#include <Net/EventLoop.h>

jing::TcpClient::TcpClient()
:m_isConnecting(false)
{
    auto defaultCallBack = [](TcpConnectionPtr){ LOG_INFO << "default callback"; };
    m_readCallBack = defaultCallBack;
    m_closeCallBack = defaultCallBack;
    m_errorCallBack = defaultCallBack;
    m_writeCallBack = defaultCallBack;
    m_connectionCallBack = defaultCallBack;
    m_mainLoop = std::make_shared<EventLoop>();}

void jing::TcpClient::connectTo(const jing::InetAddress &address)
{
    auto sockFd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int on = 1;
    setsockopt(sockFd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof on);
    if (sockFd < 0)
    {
        LOG_ERROR << __FILE__ << __LINE__ << "socket create error!\n";
        return;
    }
    m_socket.fd() = sockFd;
    //m_socket.bind(address);
    connect(m_socket.fd(), address.getSockAddr(), sizeof(struct sockaddr));
    m_connection = std::make_shared<TcpConnection>(m_mainLoop, sockFd);
    m_connection->setReadCallBack(m_readCallBack);
    m_connection->setWriteCallBack(m_writeCallBack);
    m_connection->setConnectionCallBack(m_connectionCallBack);
    m_connection->setCloseCallBack(m_closeCallBack);
    m_connection->setErrorCallBack(m_errorCallBack);
    m_isConnecting = true;
    m_mainLoop->loop();
}

jing::TcpClient::~TcpClient()
{
    if (m_isConnecting)
    {
        m_mainLoop->quit();
        m_connection->close();
    }
}


