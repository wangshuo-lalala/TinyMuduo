//
// Created by shuo on 2022/4/11.
//

#ifndef MUDUO_LOGSTREAM_H
#define MUDUO_LOGSTREAM_H

#include <Base/Copyable.h>
#include <Log/FixBuffer.h>
#include <cstring>
#include <string>


namespace jing
{
    class LogStream : public Copyable
    {
        using self = LogStream;
    public:
        self& operator<<(bool v);
        self& operator<<(int v);
        self& operator<<(ulong v);
        self& operator<<(long v);
        self& operator<<(long long v);
        self& operator<<(float v);
        self& operator<<(double v);
        self& operator<<(const char* v);
        self& operator<<(const std::string& v);

        LogBuffer& buffer()
        {
            return m_buffer;
        }
    private:
        template<class T>
        void append(T v)
        {
            std::string tmp = std::to_string(v);
            m_buffer.append(tmp.c_str(), static_cast<int>(tmp.size()));
        }
    private:
        LogBuffer m_buffer;
    };
}

#endif //MUDUO_LOGSTREAM_H
