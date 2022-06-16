//
// Created by shuo on 2022/5/1.
//
#include <Net/Socket.h>
#include <Log/Logging.h>
#include <netinet/tcp.h>
#include <unistd.h>

void jing::Socket::bind(const jing::InetAddress &address) const
{
    int ret = ::bind(m_socketFd, address.getSockAddr(), sizeof(struct sockaddr_in6));
    if (ret < 0)
    {
        LOG_ERROR << "SOCKET BIND ERROR!\n" << errno;
    }
}

void jing::Socket::listen() const
{
    int ret = ::listen(m_socketFd, SOMAXCONN);
    if (ret < 0)
    {
        LOG_ERROR << "SOCKET LISTEN ERROR!\n";
    }
}

void jing::Socket::shutDownWrite() const
{
    int ret = ::shutdown(m_socketFd, SHUT_WR);
    if (ret < 0)
    {
        LOG_ERROR << "SOCKET SHUTDOWN ERROR!\n";
    }
}

void jing::Socket::setTcpNoDelay(bool on) const
{
    ::setsockopt(m_socketFd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof on);
}

void jing::Socket::setReuseAddr(bool on) const
{
    ::setsockopt(m_socketFd, SOL_SOCKET,SO_REUSEADDR, &on, sizeof on);
}

void jing::Socket::setReusePort(bool on) const
{
    ::setsockopt(m_socketFd, SOL_SOCKET,SO_REUSEPORT, &on, sizeof on);
}

void jing::Socket::setKeepAlive(bool on) const
{
    ::setsockopt(m_socketFd, SOL_SOCKET,SO_KEEPALIVE, &on, sizeof on);
}

jing::Socket::~Socket()
{
    ::close(m_socketFd);
}

int jing::Socket::accept(jing::InetAddress *address) const
{
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof addr);
    socklen_t len = sizeof(addr);
    int connFd = ::accept4(m_socketFd, (static_cast<struct sockaddr *>(static_cast<void *>(&addr))),
                           &len, SOCK_CLOEXEC | SOCK_NONBLOCK);
    if (connFd < 0)
    {
        LOG_ERROR << "SOCKET ACCEPT ERROR!\n";
    }
    return connFd;
}