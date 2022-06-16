//
// Created by shuo on 2022/4/30.
//

#include <Net/InetAddress.h>
#include <cstring>
#include <arpa/inet.h>

jing::InetAddress::InetAddress(ushort port, bool localOnly, bool ipv6)
{
    if (ipv6)
    {
        memset(&m_addr6, 0, sizeof m_addr6);
        m_addr6.sin6_family = AF_INET6;
        in6_addr ip = (localOnly ? in6addr_loopback : in6addr_any);
        m_addr6.sin6_addr = ip;
        m_addr6.sin6_port = htons(port);
    }
    else
    {
        memset(&m_addr, 0, sizeof m_addr);
        m_addr.sin_family = AF_INET;
        in_addr_t ip = (localOnly ? INADDR_LOOPBACK : INADDR_ANY);
        m_addr.sin_addr.s_addr = htonl(ip);
        m_addr.sin_port = htons(port);
   }
}

jing::InetAddress::InetAddress(const std::string& ip, ushort port, bool ipv6)
{
    if (ipv6)
    {
        memset(&m_addr6, 0, sizeof m_addr6);
        m_addr6.sin6_family = AF_INET6;
        inet_pton(AF_INET6, ip.data(), &m_addr6.sin6_addr);
        m_addr6.sin6_port = htons(port);
    }
    else
    {
        memset(&m_addr, 0, sizeof m_addr);
        m_addr.sin_family = AF_INET;
        inet_pton(AF_INET, ip.data(), &m_addr.sin_addr);
        m_addr.sin_port = htons(port);
    }
}

void jing::InetAddress::setPort(ushort port)
{
    m_addr.sin_port = htons(port);
}
void jing::InetAddress::setIpAddress(const std::string &ip)
{
    inet_pton(AF_INET, ip.data(), &m_addr.sin_addr);
}
