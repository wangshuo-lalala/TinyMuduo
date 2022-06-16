//
// Created by shuo on 2022/4/30.
//

#ifndef TINYMUDUO_INETADDRESS_H
#define TINYMUDUO_INETADDRESS_H

#include <Base/Copyable.h>
#include <netinet/in.h>
#include <string>

namespace jing
{
    class InetAddress : public Copyable
    {
    public:
        explicit InetAddress(ushort port = 0, bool localOnly = false, bool ipv6 = false);
        explicit InetAddress(const std::string& ip, ushort port = 0, bool ipv6 = false);
        explicit InetAddress(const struct sockaddr_in& addr):m_addr(addr){}
        explicit InetAddress(const struct sockaddr_in6& addr):m_addr6(addr){}

        void setIpAddress(const std::string& ip);
        void setPort(ushort port);
        std::string getIpAddress() const {return std::to_string(ntohl(m_addr.sin_addr.s_addr));};
        ushort getPort() const {return ntohs(m_addr.sin_port);};
        sa_family_t family() const{return m_addr.sin_family;}

        const struct sockaddr* getSockAddr() const {return static_cast<const struct sockaddr*>(static_cast<const void*>(&m_addr6));}
    private:
        union
        {
            struct sockaddr_in m_addr;
            struct sockaddr_in6 m_addr6;
        };
    };
}



#endif //TINYMUDUO_INETADDRESS_H

#pragma clang diagnostic pop