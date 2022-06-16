//
// Created by shuo on 2022/4/22.
//

#ifndef TINYMUDUO_TYPEDEF_H
#define TINYMUDUO_TYPEDEF_H
#include <functional>
#include <memory>
#include <Log/Logging.h>
using TimerCallBack = std::function<void()>;
using EventCallBack = std::function<void()>;
using TimerID = int64_t;

#endif //TINYMUDUO_TYPEDEF_H
