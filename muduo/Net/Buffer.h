//
// Created by shuo on 2022/5/3.
//

#ifndef TINYMUDUO_BUFFER_H
#define TINYMUDUO_BUFFER_H
#include <Base/Copyable.h>
#include <vector>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <string>

namespace jing
{
    class Buffer : public Copyable
    {
    public:
        static const size_t kCheapPrepared = 8;
        static const size_t kInitialSize = 1024;

        explicit Buffer(size_t initialSize = kInitialSize)
        : m_buffer(kCheapPrepared + initialSize),
          m_readIndex(kCheapPrepared),
          m_writeIndex(kCheapPrepared)
        {
            assert(readableBytes() == 0);
            assert(writeableBytes() == initialSize);
            assert(prependableBytes() == kCheapPrepared);
        }


        void swap(Buffer& rhs)
        {
            m_buffer.swap(rhs.m_buffer);
            std::swap(m_writeIndex, rhs.m_writeIndex);
            std::swap(m_readIndex, rhs.m_writeIndex);
        }

        size_t readableBytes() const
        {
            return m_writeIndex - m_readIndex;
        }

        size_t writeableBytes() const
        {
            return m_buffer.size() - m_writeIndex;
        }

        size_t prependableBytes() const
        {
            return m_readIndex;
        }

        const char* peek()
        {
            return begin() + m_readIndex;
        }

        void retrieve(size_t len)
        {
            assert(len <= readableBytes());
            if (len < readableBytes())
            {
                m_readIndex += len;
            }
            else
            {
                retrieveAll();
            }
        }

        void retrieveInt64()
        {
            retrieve(sizeof(int64_t));
        }

        void retrieveInt32()
        {
            retrieve(sizeof(int32_t));
        }

        void retrieveInt16()
        {
            retrieve(sizeof(int16_t));
        }

        void retrieveInt8()
        {
            retrieve(sizeof(int8_t));
        }

        void retrieveAll()
        {
            m_readIndex = kCheapPrepared;
            m_writeIndex = kCheapPrepared;
        }

        std::string retrieveAsString(size_t len)
        {
            assert(len <= readableBytes());
            std::string res(peek(), len);
            retrieve(len);
            return res;
        }

        std::string retrieveAllAsString()
        {
            return retrieveAsString(readableBytes());
        }

        void ensureWriteable(size_t len)
        {
            if (writeableBytes() < len)
            {
                makeSpace(len);
            }
            assert(writeableBytes() >= len);
        }

        void append(const char* data, int len)
        {
            ensureWriteable(len);
            std::copy(data, data +len, begin() + m_writeIndex);
            m_writeIndex += len;
        }

        void append(const std::string& str)
        {
            ensureWriteable(str.size());
            append(str.data(), str.size());
        }

        void append(const void* data, int len)
        {
            append(static_cast<const char*>(data), len);
        }

        // append int use network endian
        void appendInt64(int64_t x)
        {
            int64_t be64 = htobe64(x);
            append(&be64, sizeof be64);
        }

        void appendInt32(int32_t x)
        {
            int32_t be32 = htobe32(x);
            append(&be32, sizeof be32);
        }

        void appendInt16(int16_t x)
        {
            int16_t be16 = htobe16(x);
            append(&be16, sizeof be16);
        }

        void appendInt8(int8_t x)
        {
            append(&x, sizeof x);
        }

        void prepend(const void* data, size_t len)
        {
            assert(len <= prependableBytes());
            m_readIndex -= len;
            const char* d = static_cast<const char*>(data);
            std::copy(d, d+len, begin());
        }

        int64_t peekInt64()
        {
            assert(readableBytes() >= sizeof(int64_t));
            int64_t res = 0;
            ::memcpy(&res, peek(), sizeof res);
            return htobe64(res);
        }

        int32_t peekInt32()
        {
            assert(readableBytes() >= sizeof(int32_t));
            int32_t res = 0;
            ::memcpy(&res, peek(), sizeof res);
            return htobe32(res);
        }

        int16_t peekInt16()
        {
            assert(readableBytes() >= sizeof(int16_t));
            int16_t res = 0;
            ::memcpy(&res, peek(), sizeof res);
            return htobe16(res);
        }

        int8_t peekInt8()
        {
            assert(readableBytes() >= sizeof(int8_t));
            int8_t res = 0;
            ::memcpy(&res, peek(), sizeof res);
            return res;
        }

        int64_t readInt64()
        {
            int64_t res = peekInt64();
            retrieveInt64();
            return res;
        }

        int32_t readInt32()
        {
            int32_t res = peekInt32();
            retrieveInt32();
            return res;
        }
        
        int16_t readInt16()
        {
            int16_t res = peekInt16();
            retrieveInt16();
            return res;
        }
        
        int8_t readInt8()
        {
            int8_t res = peekInt8();
            retrieveInt8();
            return res;
        }

        void shrink(size_t reserve)
        {
            m_buffer.shrink_to_fit();
            m_buffer.reserve(reserve);
        }
        size_t capacity()
        {
            return m_buffer.capacity();
        }

        size_t readFd(int fd, int* saveError);
    private:
        char* begin()
        {
            return m_buffer.data();
        }
        const char* begin() const
        {
            return m_buffer.data();
        }

        void makeSpace(size_t len)
        {
            if (writeableBytes() + prependableBytes() < len + kCheapPrepared)
            {
                m_buffer.resize(m_writeIndex + len);
            }
            else
            {
                assert(kCheapPrepared < m_readIndex);
                size_t readable = readableBytes();
                std::copy(begin() + m_readIndex,
                          begin() + m_writeIndex,
                          begin() + kCheapPrepared);
                m_readIndex = kCheapPrepared;
                m_writeIndex = m_readIndex + readable;
                assert(readable == readableBytes());
            }
        }


        std::vector<char> m_buffer;
        size_t m_readIndex;
        size_t m_writeIndex;
  };
}



#endif //TINYMUDUO_BUFFER_H
