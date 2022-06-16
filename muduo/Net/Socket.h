//
// Created by shuo on 2022/5/1.
//

#ifndef TINYMUDUO_SOCKET_H
#define TINYMUDUO_SOCKET_H
#include <Base/Nocopyable.h>
#include <Net/InetAddress.h>
namespace jing
{
    class Socket : public Nocopyable
    {
    public:
        explicit Socket(int socketFd = -1):m_socketFd(socketFd){}
        ~Socket();

        int& fd() {return m_socketFd;}

        void bind(const InetAddress& address) const;

        void listen() const;

        int accept(InetAddress* address) const;

        void shutDownWrite() const;

        void setTcpNoDelay(bool on) const;

        void setReuseAddr(bool on) const;

        void setReusePort(bool on) const;

        void setKeepAlive(bool on) const;


    private:
        int m_socketFd;
    };
}




#endif //TINYMUDUO_SOCKET_H
