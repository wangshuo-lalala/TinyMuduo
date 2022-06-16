//
// Created by shuo on 2022/4/15.
//

#ifndef MUDUO_FIXBUFFER_H
#define MUDUO_FIXBUFFER_H
#include <string>
#include <cstring>

namespace jing
{
    template <int SIZE>
    class FixBuffer
    {
    public:
        FixBuffer()
        {
            clear();
        }
        ~FixBuffer() = default;

        int length()
        {
            return static_cast<int>(m_cur - m_data);
        }

        int avail()
        {
            return static_cast<int>(end() - m_cur);
        }

        bool append(const char* buf, int length)
        {
            if (avail() > length)
            {
                memcpy(m_cur, buf, length);
                m_cur += length;
                return true;
            }

            return false;
        }

        const char* data()
        {
            return  m_data;
        }

        std::string toString()
        {
            return std::string(m_data, length());
        }

        void clear()
        {
            memset(m_data, 0, sizeof m_data);
            m_cur = m_data;
        }
    private:
        char* end()
        {
            return m_data + sizeof m_data;
        }

    private:
        char m_data[SIZE]{0};
        char* m_cur{nullptr};
    };
    const int SmallFixBuffer = 1024;
    const int LargeFixBuffer = 1024 * 1024;
    using LogBuffer = FixBuffer<SmallFixBuffer>;
    //using LargerBuffer = FixBuffer<SmallFixBuffer>;
}
#endif //MUDUO_FIXBUFFER_H
