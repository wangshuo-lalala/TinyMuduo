//
// Created by shuo on 2022/4/16.
//

#include <Net/PollEventHandler.h>
#include <Net/EventCallBack.h>
#include <Net/EventLoop.h>
#include <Log/Logging.h>
jing::PollEventHandler::PollEventHandler(int fd)
:m_fd(fd),
m_isHandling(false),
m_events(0),
m_revents(0)
{

}

void jing::PollEventHandler::setReadCallBack(EventCallBack callBack)
{
    m_readCallBack = std::move(callBack);
}

void jing::PollEventHandler::setWriteCallBack(EventCallBack callBack)
{
    m_writeCallBack = std::move(callBack);
}

void jing::PollEventHandler::setCloseCallBack(EventCallBack callBack)
{
    m_closeCallBack = std::move(callBack);
}

void jing::PollEventHandler::setErrorCallBack(EventCallBack callBack)
{
    m_errorCallBack = std::move(callBack);
}

void jing::PollEventHandler::handleEvent(TimeStamp stamp)
{
    m_isHandling = true;
    if ((m_revents & POLLHUP) && !(m_revents & POLLIN))
    {
        if (m_closeCallBack) m_closeCallBack();
    }
    if (m_revents & POLLNVAL)
    {
        LOG_WARN << "fd=" << m_fd << "handle event() POLLNVAL";
    }
    if (m_revents & POLLERR)
    {
        if (m_errorCallBack) m_errorCallBack();
    }
    if (m_revents & (POLLIN | POLLRDHUP | POLLPRI))
    {
        if (m_readCallBack) m_readCallBack();
    }
    if (m_revents & (POLLOUT))
    {
        if (m_writeCallBack) m_writeCallBack();
    }
    m_isHandling = false;
}


