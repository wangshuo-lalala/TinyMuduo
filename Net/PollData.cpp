//
// Created by shuo on 2022/4/16.
//

#include <Net/PollData.h>
#include <Net/EventCallBack.h>
#include <Net/EventLoop.h>

void jing::PollData::setReadCallBack(std::shared_ptr<EventCallBack> callBack)
{
    m_readCallBack = std::move(callBack);
}

void jing::PollData::setWriteCallBack(std::shared_ptr<EventCallBack> callBack)
{
    m_writeCallBack = std::move(callBack);
}

void jing::PollData::setCloseCallBack(std::shared_ptr<EventCallBack> callBack)
{
    m_closeCallBack = std::move(callBack);
}

void jing::PollData::setErrorCallBack(std::shared_ptr<EventCallBack> callBack)
{
    m_errorCallBack = std::move(callBack);
}

void jing::PollData::handleEvent()
{

}
