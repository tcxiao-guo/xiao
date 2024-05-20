/**
 * @file Logger.cpp
 * @author xiao guo
 * @brief
 * @version 0.1
 * @date 2024-05-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <xiao/utils/Logger.h>
#include <assert.h>

namespace xiao
{
    // helper class for known string length at compile time

    class T
    {
    public:
        T(const char *str, unsigned len) : str_(str), len_(len)
        {
            assert(strlen(str) == len_);
        }

        const char *str_;
        const unsigned len_;
    };

    const char *strerror_tl(int savedErrno)
    {
#ifndef _MSC_VER
        return strerror(savedErrno);
#else
        static thread_local char errMsg[64];
        (void)strerror_s<sizeof errMsg>(errMsg, savedErrno);
        return errMsg;
#endif
    }

    inline LogStream &operator<<(LogStream &s, T v)
    {
        s.append(v.str_, v.len_);
        return s;
    }

    inline LogStream &operator<<(LogStream &s, const Logger::SourceFile &v)
    {
        s.append(v.data_, v.size_);
        return s;
    }
}

using namespace xiao;

static thread_local uint64_t lastSecond_{0};
static thread_local char lastTimeString_[32] = {0};
#ifdef __linux__
static thread_local pid_t threadId_{0};
#else
static thread_local uint64_t threadId_{0};
#endif
//  static thread_local LogStream logStream_;

void Logger::formatTime()
{
    uint64_t now = static_cast<uint64_t>(date_.secondsSinceEpoch());
    uint64_t microSec = static_cast<uint64_t>(date_.microSecondsSinceEpoch() -
                                              date_.roundSecond().microSecondsSinceEpoch());
    if (now != lastSecond_)
    {
        lastSecond_ = now;
        if (displayLocalTime_())
        {
#ifndef _MSC_VER
            strncpy(lastTimeString_,
                    date_.toFormattedStringLocal(false).c_str(),
                    sizeof(lastTimeString_) - 1);
#else
            strncpy_s<sizeof lastTimeString_>(
                lastTimeString_,
                date_.toFormattedStringLocal(false).c_str(),
                sizeof(lastTimeString_) - 1);
#endif
        }
        else
        {
#ifndef _MSC_VER
            strncpy(lastTimeString_,
                    date_.toFormattedString(false).c_str(),
                    sizeof(lastTimeString_) - 1);
#else
            strncpy_s<sizeof lastTimeString_>(
                lastTimeString_,
                date_.toFormattedString(false).c_str(),
                sizeof(lastTimeString_) - 1);
#endif
        }
    }
    logStream_ << T(lastTimeString_, 17);
}