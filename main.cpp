#include <Log/Logging.h>
#include <Log/AsynLogEvent.h>
#include <Log/Date.h>
int main()
{

    std::shared_ptr<jing::ILogEvent> event = std::make_shared<jing::AsynLogEvent>();
    //LogInfoInstance.useDefault();
    LogInfoInstance.addLogEvent(event);
    {
        jing::TimeCount tiemCount;
        for (int i = 0; i < 5000000; ++i)
        {
            LOG_DEBUG << "ABCdf" << 120 << __FILE__ << "\n";
            LOG_INFO << "abcvdfa" << 120 << __FILE__ << "\n";
            LOG_ERROR << "abcvdfa" << 120 << __FILE__ << "\n";
            LOG_WARN << "abcvdfa" << 120 << __FILE__ << "\n";
        }
    }

}