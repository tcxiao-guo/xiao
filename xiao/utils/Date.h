/**
 * @file Date.h
 * @author Xiao Guo
 * @brief
 * @version 0.1
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <xiao/exports.h>
#include <stdint.h>
#include <string>

#define MICRO_SECONDS_PRE_SEC 1000000LL

namespace xiao
{
    /**
     * @brief This class represents a time point.
     *
     */
    class XIAO_EXPORT Date
    {
    public:
        Date() : microSecondsSinceEpoch_(0){};

        /**
         * @brief Construct a new Date instance.
         *
         * @param microSec The microseconds from 1970-01-01 00:00:00
         */
        explicit Date(int64_t microSec) : microSecondsSinceEpoch_(microSec){};

        /**
         * @brief Construct a new Date instance.
         *
         * @param year
         * @param month
         * @param day
         * @param hour
         * @param minute
         * @param second
         * @param microSecond
         */
        Date(uint32_t year,
             uint32_t month,
             uint32_t day,
             uint32_t hour = 0,
             uint32_t minute = 0,
             uint32_t second = 0,
             uint32_t microSecond = 0);

        /**
         * @brief Create a Date object that represents the current time.
         *
         * @return const Date
         */
        static const Date date();

        /**
         * @brief Same as the date() method.
         *
         * @return const Date
         */
        static const Date now()
        {
            return Date::date();
        }

        static int64_t timezoneOffset()
        {
            static int64_t offset = -(
                Date::fromDbStringLocal("1970-01-03 00:00:00").secondsSinceEpoch() -
                2LL * 3600LL * 24LL);
            return offset;
        }

        /**
         * @brief Return a new Date instance that represents the time after some
         * seconds from *this.
         *
         * @param second
         * @return const Date
         */
        const Date after(double second) const;

        /**
         * @brief Return a new Date instance that equals to *this, but with zero
         * microseconds.
         *
         * @return const Date
         */
        const Date roundSecond() const;

        /**
         * @brief Return a new Date instance that equals to *this, but with zero
         * hours, minutes, seconds and microseconds.
         *
         * @return const Date
         */
        const Date roundDay() const;

        ~Date(){};

        /**
         * @brief Return true if the time point is equal to another.
         *
         */
        bool operator==(const Date &date) const
        {
            return microSecondsSinceEpoch_ == date.microSecondsSinceEpoch_;
        }

        /**
         * @brief Return true if the time point is not equal to another.
         *
         */
        bool operator!=(const Date &date) const
        {
            return microSecondsSinceEpoch_ != date.microSecondsSinceEpoch_;
        }

        /**
         * @brief Return true if the time point is earlier than another.
         *
         */
        bool operator<(const Date &date) const
        {
            return microSecondsSinceEpoch_ < date.microSecondsSinceEpoch_;
        }

        /**
         * @brief Return true if the time point is later than another.
         *
         */
        bool operator>(const Date &date) const
        {
            return microSecondsSinceEpoch_ > date.microSecondsSinceEpoch_;
        }

        /**
         * @brief Return true if the time point is not earlier than another.
         *
         */
        bool operator>=(const Date &date) const
        {
            return microSecondsSinceEpoch_ >= date.microSecondsSinceEpoch_;
        }

        /**
         * @brief Return true if the time point is not later than another.
         *
         */
        bool operator<=(const Date &date) const
        {
            return microSecondsSinceEpoch_ <= date.microSecondsSinceEpoch_;
        }

        /**
         * @brief Get the number of milliseconds since 1970-01-01 00:00.
         *
         * @return int64_t
         */
        int64_t microSecondsSinceEpoch() const
        {
            return microSecondsSinceEpoch_;
        }

        /**
         * @brief Get the number of seconds since 1970-01-01 00:00.
         *
         * @return int64_t
         */
        int64_t secondsSinceEpoch() const
        {
            return microSecondsSinceEpoch_ / MICRO_SECONDS_PRE_SEC;
        }

        /**
         * @brief Get the tm struct for the time point.
         *
         * @return struct tm
         */
        struct tm tmStruct() const;

        /**
         * @brief Generate a UTC time string
         * @param showMicroseconds whether the microseconds are returned.
         * @note Examples:
         *   - "20240323 12:11:21" if the @p showMicroseconds is false
         *   - "20240323 12:11:21:102414" if the @p showMicroseconds is true
         */
        std::string toFormattedString(bool showMicroseconds) const;

        /**
         * @brief Generate a UTC time string formated by the @p fmtStr
         * @param fmtStr is the format string for the function strftime()
         * @param showMicrosecnods whether the microseconds are returned.
         * @note Examples:
         *   - "20230323 12:11:21" if the @p fmtStr is "%Y-%m-%d %H:%M:%S" and the
         *      @p showMicroseconds is false
         *   - "20230323 12:11:21:102414" if the @p fmtStr is "%Y-%m-%d %H:%M:%S" and the
         *      @p showMicroseconds is true
         */
        std::string toCustomedFormattedString(const std::string &fmtStr,
                                              bool showMicrosecnods = false) const;

        /**
         * @brief Generate a local time zone string, the format of the string is
         * same as the mothed toFormattedString
         *
         * @param showMicroseconds
         * @return std::string
         */
        std::string toFormattedStringLocal(bool showMicroseconds) const;

        /**
         * @brief Generate a local time zone string formated by the @p fmtStr
         *
         * @param fmtStr
         * @param showMicroseconds
         * @return std::string
         */
        std::string toCustomedFormattedStringLocal(
            const std::string &fmtStr,
            bool showMicroseconds = false) const;

        /**
         * @brief Generate a local time zone string for database.
         * @note Examples:
         *  - "2018-01-01" if hours, minutes, seconds and microseconds are zero
         *  - "2018-01-01 10:10:25" if the microsecond is zero
         *  - "2018-01-01 10:10:25:102414" if the microsecond is not zero
         */
        std::string toDbStringLocal() const;
        /**
         * @brief Generate a UTC time string for database.
         */
        std::string toDbString() const;

        /**
         * @brief From DB string to xiao local time zone.
         *
         * Inverse of toDbStringLocal()
         */
        static Date fromDbStringLocal(const std::string &datetime);
        /**
         * @brief From DB string to trantor UTC time.
         *
         * Inverse of toDbString()
         */
        static Date fromDbString(const std::string &datetime);

        /**
         * @brief Generate a UTC time string.
         *
         * @param fmtStr The format string.
         * @param str The string buffer for the generated time string.
         * @param len The length of the string buffer.
         */
        void toCustomedFormattedString(const std::string &fmtStr,
                                       char *str,
                                       size_t len) const; // UTC

        /**
         * @brief Return true if the time point is in a same second as another.
         *
         * @param date
         * @return true
         * @return false
         */
        bool isSameSecond(const Date &date) const
        {
            return microSecondsSinceEpoch_ / MICRO_SECONDS_PRE_SEC ==
                   date.microSecondsSinceEpoch_ / MICRO_SECONDS_PRE_SEC;
        }

        /**
         * @brief Swap the time point with anther.
         *
         * @param that
         */
        void swap(Date &that)
        {
            std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
        }

    private:
        int64_t microSecondsSinceEpoch_{0};
    };
}