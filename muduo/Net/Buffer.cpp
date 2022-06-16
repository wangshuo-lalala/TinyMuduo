//
// Created by shuo on 2022/5/3.
//

#include <Net/Buffer.h>
#include <sys/uio.h>

size_t jing::Buffer::readFd(int fd, int* saveError)
{
    char extrabuf[65535];
    struct iovec vec[2];
    size_t writable = writeableBytes();
    vec[0].iov_base = begin() + m_writeIndex;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;
    int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
    auto n = ::readv(fd, vec, iovcnt);
    if (n < 0)
    {
        *saveError = errno;
    }
    else if (static_cast<size_t>(n) <= writable)
    {
        m_writeIndex += n;
    }
    else
    {
        m_writeIndex = m_buffer.size();
        append(extrabuf, n - writable);
    }
    return n;
}
