/**
 * @file NonCopyable.h
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

namespace xiao
{
    /**
     * @brief  This class represents a non-copyable object.
     *
     */
    class XIAO_EXPORT NonCopyable
    {
    protected:
        NonCopyable()
        {
        }
        ~NonCopyable()
        {
        }
        NonCopyable(const NonCopyable &) = delete;
        NonCopyable &operator=(const NonCopyable &) = delete;

        NonCopyable(NonCopyable &&) noexcept(true) = default;
        NonCopyable &operator=(NonCopyable &&) noexcept(true) = default;
    };

}