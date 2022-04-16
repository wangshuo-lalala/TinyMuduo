//
// Created by shuo on 2022/4/11.
//
#include <Log/LogStream.h>

jing::LogStream::self &jing::LogStream::operator<<(bool v)
{
    append(v);
    return *this;
}

jing::LogStream::self &jing::LogStream::operator<<(int v)
{
    append(v);
    return *this;
}

jing::LogStream::self &jing::LogStream::operator<<(float v)
{
    append(v);
    return *this;
}

jing::LogStream::self &jing::LogStream::operator<<(double v)
{
    append(v);
    return *this;
}

jing::LogStream::self &jing::LogStream::operator<<(const char *v)
{
    std::string tmp(v);
    m_buffer.append(tmp.c_str(), static_cast<int>(tmp.size()));
    return *this;
}

jing::LogStream::self &jing::LogStream::operator<<(const std::string &v)
{
    m_buffer.append(v.c_str(), static_cast<int>(v.size()));
    return *this;
}
