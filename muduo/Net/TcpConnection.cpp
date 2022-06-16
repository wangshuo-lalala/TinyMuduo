//
// Created by shuo on 2022/5/4.
//

#include <Net/TcpConnection.h>
#include <Net/EventLoop.h>
#include <Net/PollEventHandler.h>
#include <unistd.h>
jing::TcpConnection::TcpConnection(std::shared_ptr<EventLoop> loop, int socketFd)
:m_loop(std::move(loop)),
m_socket(new Socket(socketFd)),
m_isClosed(false)
{
    m_socket->setKeepAlive(true);
    m_handler = std::make_shared<PollEventHandler>(socketFd);
    m_handler->setReadCallBack([this]()
                               {
                                  handleRead();
                               });
    m_handler->setWriteCallBack([this]()
                               {
                                   handleWrite();
                               });
    m_handler->setErrorCallBack([this]()
                               {
                                   handleError();
                               });
    m_handler->setCloseCallBack([this]()
                               {
                                   handleClose();
                               });
    m_loop->runInloop([this]()
                      {
                          connectEstablished();
                      });

}

void jing::TcpConnection::connectEstablished()
{
    m_handler->enableReading();
    m_loop->addHandler(m_handler);
    m_connectionCallBack(shared_from_this());
//    m_timeoutTimer = m_loop->runEvery(20, [this]()
//    {
//        auto duration = timeStamp().now().secondSinceEpoch() - m_timeStamp.secondSinceEpoch();
//        if (duration > 10)
//        {
//            close();
//        }
//    });
}

void jing::TcpConnection::connectDestroyed()
{

}

void jing::TcpConnection::handleRead()
{
    m_readCallBack(shared_from_this());
}

void jing::TcpConnection::handleWrite()
{
    m_loop->assertInLoopThread();
    if (m_handler->isWriting() && m_outputBuffer.readableBytes() > 0)
    {
        auto n = ::write(m_socket->fd(), m_outputBuffer.peek(), m_outputBuffer.readableBytes());
        if (n > 0)
        {
            m_outputBuffer.retrieve(n);
            if (m_outputBuffer.readableBytes() == 0)
            {
                m_handler->disableWriting();
            }
        }
        m_writeCallBack(shared_from_this());
    }
    else
    {
        m_handler->disableWriting();
    }
}

void jing::TcpConnection::handleClose()
{
    m_closeCallBack(shared_from_this());
}

void jing::TcpConnection::handleError()
{
    m_errorCallBack(shared_from_this());
}

std::string jing::TcpConnection::readAll()
{
    m_loop->assertInLoopThread();
    int error;
    auto n = m_inputBuffer.readFd(m_socket->fd(), &error);
    if (n == 0)
    {
        close();
    }
    else if (n < 0)
    {
        handleError();
    }
    m_timeStamp.updateToNow();
    return m_inputBuffer.retrieveAllAsString();
}

void jing::TcpConnection::close()
{
    shutdown(m_socket->fd(),SHUT_WR);
    m_loop->assertInLoopThread();
    m_handler->disableAll();
    m_loop->removeHandler(m_handler);
    m_isClosed = true;
//    m_loop->cancelTimer(m_timeoutTimer);
}

bool jing::TcpConnection::isclosed()
{
    return m_isClosed;
}

void jing::TcpConnection::send(std::string text)
{
    m_loop->assertInLoopThread();
    if (m_handler->isWriting() || m_outputBuffer.readableBytes() != 0)
    {
        return;
    }
    auto n = ::write(m_socket->fd(), text.data(), text.size());
    if (n < 0)
    {
        LOG_ERROR << "write error!";
    }
    else if (n < text.size())
    {
        m_outputBuffer.append(text.data() + n, text.size() - n);
        m_handler->enableWriting();
    }
}
