//
// Created by shuo on 2022/4/12.
//
#include <Log/Logging.h>
#include <Log/Date.h>

jing::Logging::~Logging()
{
    int length = m_stream.buffer().length();
    LogInfoInstance.traverseLogging(m_stream.buffer().data(), length);
}

jing::LogStream &jing::Logging::stream()
{
    return m_stream;
}

jing::Logging::Logging(int level)
{
    m_stream << jing::now();
    if (LogInfoInstance.isDefault())
    {
        switch (level)
        {
        case DEBUG_LEVEL:
            m_stream <<"\033[33m[debug]\033[0m ";
            break;
        case INFO_LEVEL:
            m_stream << "\033[1m[info]\033[0m ";
            break;
        case WARN_LEVEL:
            m_stream << "\033[34m[warn]\033[0m ";
            break;
        case ERROR_LEVEL:
            m_stream << "\033[31m[error]\033[0m ";
            break;
        default:
            break;
        }
    }
    else
    {
        switch (level)
        {
            case DEBUG_LEVEL:
                m_stream <<"[debug] ";
                break;
            case INFO_LEVEL:
                m_stream << "[info] ";
                break;
            case WARN_LEVEL:
                m_stream << "[warn] ";
                break;
            case ERROR_LEVEL:
                m_stream << "[error] ";
                break;
            default:
                break;
        }
    }
}

