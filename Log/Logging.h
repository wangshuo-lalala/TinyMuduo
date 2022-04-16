//
// Created by shuo on 2022/4/12.
//

#ifndef MUDUO_LOGGING_H
#define MUDUO_LOGGING_H
#include <vector>
#include <memory>
#include <Log/ILogEvent.h>
#include <Log/LogStream.h>
#include <Log/LogStdOut.h>
#include <thread>
#include <mutex>
namespace jing {
    static const int DEBUG_LEVEL = 0;
    static const int INFO_LEVEL = 1;
    static const int WARN_LEVEL = 2;
    static const int ERROR_LEVEL = 3;

    class LogInfo
    {
    public:
        static LogInfo* instance()
        {
            static LogInfo instance;
            return &instance;
        }

        void setLevel(int level)
        {
            std::unique_lock<std::mutex> l(m_mutex);
            m_level = level;
        }

        int getLevel()
        {
            std::unique_lock<std::mutex> l(m_mutex);
            return m_level;
        }

        void addLogEvent(const std::shared_ptr<ILogEvent>& event)
        {
            m_logEvents.emplace_back(event);
        }

        void removeLogEvent(const std::shared_ptr<ILogEvent>& event)
        {
            for (auto item = m_logEvents.begin(); item != m_logEvents.end(); ++item)
            {
                if (*item == event)
                {
                    m_logEvents.erase(item);
                    break;
                }
            }
        }

        void clearLogEvent()
        {
            m_logEvents.clear();
        }

        void traverseLogging(const char* buf, int length)
        {
            for (auto& item : m_logEvents)
            {
                item->log(buf, length);
            }
        }

        void useDefault()
        {
            std::shared_ptr<jing::ILogEvent> logEvent = std::make_shared<LogStdOut>();
            addLogEvent(logEvent);
            m_useDefault = true;
        }

        bool isDefault() const
        {
            return m_useDefault && (m_logEvents.size() == 1);
        }
    private:
        LogInfo() = default;
        ~LogInfo() = default;

        std::vector<std::shared_ptr<ILogEvent> > m_logEvents;
        int m_level{DEBUG_LEVEL};
        bool m_useDefault{false};
        std::mutex m_mutex;
    };

    class Logging
    {
    public:
        explicit Logging(int level);
        ~Logging();

        LogStream& stream();
    private:
        LogStream m_stream;
    };
}

#define LogInfoInstance (*(jing::LogInfo::instance()))

#define LOG_DEBUG if (LogInfoInstance.getLevel() <= jing::DEBUG_LEVEL) \
        jing::Logging(jing::DEBUG_LEVEL).stream()
#define LOG_INFO if (LogInfoInstance.getLevel() <= jing::INFO_LEVEL) \
        jing::Logging(jing::INFO_LEVEL).stream()
#define LOG_WARN if (LogInfoInstance.getLevel() <= jing::WARN_LEVEL) \
        jing::Logging(jing::WARN_LEVEL).stream()
#define LOG_ERROR if (LogInfoInstance.getLevel() <= jing::ERROR_LEVEL) \
        jing::Logging(jing::ERROR_LEVEL).stream()
#endif //MUDUO_LOGGING_H
