/**
 * @file Logger.h
 * @author Xiao Guo
 * @brief
 * @version 0.1
 * @date 2024-03-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <xiao/utils/NonCopyable.h>
#include <xiao/exports.h>
#include <xiao/utils/LogStream.h>
#include <functional>
#include <memory>

namespace spdlog
{
    class logger;
}

namespace xiao
{
    /**
     * @brief This class implements log functions.
     *
     */
    class XIAO_EXPORT Logger : public NonCopyable
    {
    public:
        enum LogLevel
        {
            xTrace = 0,
            xDebug,
            xInfo,
            xWarn,
            xError,
            xFatal,
            xNumberofLogLevels
        };

        /**
         * @brief Calculate of basename of source files in compile time.
         *
         */
        class SourceFile
        {
        public:
            template <int N>
            inline SourceFile(const char (&arr)[N]) : data_(arr), size_(N - 1)
            {
#ifndef _MSC_VER
                const char *slash = strrchr(data_, '/');
#else
                const char *slash = strrchr(data_, '\\');
#endif
                if (slash)
                {
                    data_ = slash + 1;
                }
                size_ = static_cast<int>(strlen(data_));
            }

            const char *data_;
            int size_;
        };

        Logger(SourceFile file, int line);
        Logger(SourceFile file, int line, LogLevel level);
        Logger(SourceFile file, int line, bool isSysErr);
        Logger(SourceFile file, int line, LogLevel level, const char *func);

        Logger();
        Logger(LogLevel level);
        Logger(bool isSysErr);

        ~Logger();
        Logger &setIndex(int index)
        {
            index_ = index;
            return *this;
        }
        LogStream &stream();

        /**
         * @brief Set the output function.
         *
         * @param outputFunc The function to output a log message.
         * @param flushFunc The function to flush.
         * @param index The channel index.
         * @note Logs are output to the standard output by default.
         */
        static void setOutputFunction(
            std::function<void(const char *msg, const uint64_t len)> outputFunc,
            std::function<void()> flushFunc,
            int index = -1)
        {
            if (index < 0)
            {
                outputFunc_() = outputFunc;
                flushFunc_() = flushFunc;
            }
            else
            {
                outputFunc_(index) = outputFunc;
                flushFunc_(index) = flushFunc;
            }
        }

        /**
         * @brief Set the log level. Logs below the level are not printed.
         *
         * @param level
         */
        static void setLogLevel(LogLevel level)
        {
            logLevel_() = level;
        }

        /**
         * @brief Get the current log level.
         *
         * @return LogLevel
         */
        static LogLevel logLevel()
        {
            return logLevel_();
        }

        /**
         * @brief Check whether it shows local time or UTC time.
         *
         */
        static bool displayLocalTime()
        {
            return displayLocalTime_;
        }

        /**
         * @brief Set whether it shows local time or UTC time. the default is UTC
         *
         */
        static void setDisplayLocalTime(bool showLocalTime)
        {
            displayLocalTime_() = showLocalTime;
        }

        /**
         * @brief Check whether xiao was build with spdlog support
         * @retval true if yes
         * @retval false if not - in this case, all the spdlog functions are noop functions
         *
         */
        static bool hasSpdLogSupport();
        /**
         * @brief Enable logging with spdlog for the specified channel.
         * @param index channel index (-1 = default channel).
         * @param logger spdlog::logger object to use.
         *              If none given, defaults to getDefaultSpdLogger(@p index).
         * @remarks If provided, it is not registered with the spdlog logger
         *          registry, it's up to you to register/drop it.
         */
        static void enableSpdLog(int index,
                                 std::shared_ptr<spdlog::logger> logger = {});

        /**
         * @brief Enable logging with spdlog for the default channel.
         * @param logger spdlog::logger object to use.
         *              If none given, defaults to getDefaultSpdLogger().
         * @remarks If provided, it is not registered with the spdlog logger
         *          registry, it's up to you to register/drop it.
         *
         */
        inline static void enableSpdLog(std::shared_ptr<spdlog::logger> logger = {})
        {
            enableSpdLog(-1, logger);
        }
        /**
         * @brief Disable logging with spdlog for the specified channel.
         * @param[in] channel index (-1 = default channel).
         * @remarks The spdlog::logger object is unregistered and
         *          destroyed only if it was created by
         *          getDefaultSpdLogger(@p index).
         *          Custom loggers are only unset.
         *
         */
        static void disableSpdLog(int index);
        /**
         * @brief Disable logging with spdlog for the default channel
         * @remarks The spdlog::logger object is unregistered and
         *          destroyed only if it was created by getDefaultSpdLogger().
         *          Custom loggers are only unset.
         */
        static void disableSpdLog()
        {
            disableSpdLog(-1);
        }
        /**
         * @brief Get the spdlog::logger set on the specified channel.
         * @param[in] channel index (-1 = default channel).
         * @return the logger, if set, else a null pointer.
         */
        static std::shared_ptr<spdlog::logger> getSpdLogger(int index = -1);
        /**
         * @brief Get a default spdlog::logger for the specified channel.
         * @details This helper function provides a default spdlog::logger with a
         *          similar output format as the existing non-spdlog trantor::Logger
         *          format.
         *
         *          If a default logger was already created for the channel, it is
         *          returned as-is.
         *
         *          Otherwise, a new spdlog::logger object named "trantor" (for
         *          index < 0) or "trantor<channel>" is created, registered with
         *          spdlog, and configured as follows:
         *          - it has the same sinks as the lowest (index) enabled channel,
         *            or those of the spdlog::default_logger(), which by defaults
         *            outputs to stdout (spdlog::sinks::stdout_color_mt),
         *          - its format pattern is set to resemble to the existing
         *            non-spdlog trantor::Logger format
         *            ("%Y%m%d %T.%f %6t %^%=8l%$ [%!] %v - %s:%#"),
         *          - the logging level is set to unfiltered (spdlog::level::trace)
         *            since the internal trantor/drogon level filtering is still
         *            managed by trantor:::Logger,
         *          - the flush level is set to spdlog::level::error.
         * @note To add custom sinks to all the channels, you can do that this way:
         *        -# (optional) add your sinks to spdlog::default_logger(),
         *        -# create the default logger for the default channel using
         *           getDefaultSpdLogger(-1),
         *        -# if not done at step 1., add your sinks to this logger,
         *        -# enable the logger with enableSpdLog(),
         *        -# for the other channels, invoke enableSpdLog(index).
         * @remarks The created spdlog::logger is automatically registered
         *          with the spdlog logger registry.
         * @param[in] channel index (-1 = default channel).
         * @return the default spdlog logger for the channel.
         */
        static std::shared_ptr<spdlog::logger> getDefaultSpdLogger(int index);

    protected:
        static void defaultOutputFunction(const char *msg, const uint64_t len)
        {
            fwrite(msg, 1, static_cast<size_t>(len), stdout);
        }
        static void defaultFlushFunction()
        {
            fflush(stdout);
        }
        void formatTime();
        static bool &displayLocalTime_()
        {
            static bool showLocalTime = false;
            return showLocalTime;
        }

        static LogLevel &logLevel_()
        {
#ifdef RELEASE
            static LogLevel logLevel = LogLevel::xInfo;
#else
            static LogLevel logLevel = LogLevel::xDebug;
#endif
            return logLevel;
        }
        static std::function<void(const char *msg, const uint64_t len)> &outputFunc_()
        {
            static std::function<void(const char *msg, const uint64_t len)>
                outputFunc = Logger::defaultOutputFunction;
            return outputFunc;
        }
        static std::function<void()> &flushFunc_()
        {
            static std::function<void()> flushFunc = Logger::defaultFlushFunction;
            return flushFunc;
        }
        static std::function<void(const char *msg, const uint64_t len)> &outputFunc_(size_t index)
        {
            static std::vector<
                std::function<void(const char *msg, const uint64_t len)>>
                outputFuncs;
            if (index < outputFuncs.size())
            {
                return outputFuncs[index];
            }
            while (index >= outputFuncs.size())
            {
                outputFuncs.emplace_back(outputFunc_());
            }
            return outputFuncs[index];
        }
        static std::function<void()> &flushFunc_(size_t index)
        {
            static std::vector<std::function<void()>> flushFuncs;
            if (index < flushFuncs.size())
            {
                return flushFuncs[index];
            }
            while (index >= flushFuncs.size())
            {
                flushFuncs.emplace_back(flushFunc_());
            }
            return flushFuncs[index];
        }

        friend class RawLogger;
        LogStream logStream_;
        LogLevel level_;
        int index_{-1};
    };
    class XIAO_EXPORT RawLogger : public NonCopyable
    {
    public:
        ~RawLogger();
        RawLogger &setIndex(int index)
        {
            index_ = index;
            return *this;
        }
        LogStream &stream()
        {
            return logStream_;
        }

    private:
        LogStream logStream_;
        int index_{-1};
    };
}