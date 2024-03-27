/**
 * @file AsyncStream.h
 * @author Xiao Guo
 * @brief
 * @version 0.1
 * @date 2024-03-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <xiao/utils/NonCopyable.h>
#include <string>
#include <memory>

namespace xiao
{
    /**
     * @brief This class represents a data stream that can be sent asynchronously.
     * The data is sent in chunks, and the chunks are sent in order, and all the
     * chunks are sent continuously.
     */
    class XIAO_EXPORT AsyncStream : public NonCopyable
    {
    public:
        virtual ~AsyncStream() = default;
        /**
         * @brief Send data asynchronously.
         *
         * @param data The data to be sent
         * @param len The length of the data
         * @return true if the data is sent successfully or at least is put in send buffer.
         * @return false if the connection is closed.
         *
         */
        virtual bool send(const char *data, size_t len) = 0;
        bool send(const std::string &data)
        {
            return send(data.data(), data.length());
        }
        /**
         * @brief Terminate the steam.
         *
         */
        virtual void close() = 0;
    };
    using AsyncStreamPtr = std::unique_ptr<AsyncStream>;
}