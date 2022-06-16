//
// Created by shuo on 2022/5/2.
//

#include <Net/TcpServer.h>
#include <Net/EventLoopThreadPool.h>
#include <Net/EventLoopThread.h>
#include <Net/EventLoop.h>
#include <Log/Logging.h>
#include <Net/PollEventHandler.h>
jing::TcpServer::TcpServer(int threadNum)

{
    auto defaultCallBack = [](TcpConnectionPtr){ LOG_INFO << "default callback"; };
    m_readCallBack = defaultCallBack;
    m_closeCallBack = defaultCallBack;
    m_errorCallBack = defaultCallBack;
    m_writeCallBack = defaultCallBack;
    m_connectionCallBack = defaultCallBack;
    m_loopPool = std::make_unique<EventLoopThreadPool>(threadNum);
    m_mainLoop = std::make_unique<EventLoop>();
}

void jing::TcpServer::listen(const jing::InetAddress &address)
{
    int sockFd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int on = 1;
    setsockopt(sockFd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof on);
    if (sockFd < 0)
    {
        LOG_ERROR << __FILE__ << __LINE__ << "socket create error!\n";
        return;
    }
    m_listenSocket.fd() = sockFd;
    m_listenSocket.bind(address);
    m_listenSocket.listen();

    m_mainLoop->runInloop([=]()
                          {
                              std::shared_ptr<PollEventHandler> handler = std::make_shared<PollEventHandler>(m_listenSocket.fd());
                              handler->enableReading();
                              handler->setReadCallBack([this]()
                                                       {
                                                           newConnection();
                                                       });
                              m_mainLoop->addHandler(handler);
                          });
    m_mainLoop->runEvery(10, [this]()
    {
        for (auto itr = m_connectionMap.begin(); itr != m_connectionMap.end(); ++itr)
        {
            if (itr->second->isclosed())
            {
                m_connectionMap.erase(itr);
                break;
            }
        }
    });
    m_loopPool->start();
    m_mainLoop->loop();
}

void jing::TcpServer::newConnection()
{
    LOG_INFO << "new connection";
    InetAddress address;
    auto connfd = m_listenSocket.accept(&address);
    auto ioLoop = m_loopPool->getFitLoop();
    auto connection = std::make_shared<TcpConnection>(ioLoop, connfd);
    connection->setReadCallBack(m_readCallBack);
    connection->setWriteCallBack(m_writeCallBack);
    connection->setConnectionCallBack(m_connectionCallBack);
    connection->setCloseCallBack(m_closeCallBack);
    connection->setErrorCallBack(m_errorCallBack);
    m_connectionMap[connfd] = connection;
}

jing::TcpServer::~TcpServer()
{
    if (m_isListening)
    {
        close();
    }
}

void jing::TcpServer::close()
{
    m_mainLoop->quit();
    for (auto connetion : m_connectionMap)
    {
        connetion.second->close();
    }
    m_connectionMap.clear();
}

bool jing::TcpServer::isListening() const
{
    return m_isListening;
}
