/**
 * @file TaskQueue.h
 * @author Xiao Guo
 * @brief
 * @version 0.1
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "NonCopyable.h"
#include <functional>
#include <string>
#include <future>

namespace xiao
{
    /**
     * @brief This class is a pure virtual class that can be implemented as a
     * SerialTaskQueue or a ConcurrentTaskQueue.
     *
     */
    class TaskQueue : public NonCopyable
    {
    public:
        virtual void runTaskInQueue(const std::function<void()> &task) = 0;
        virtual void runTaskInQueue(std::function<void> &&task) = 0;
        virtual std::string getName() const
        {
            return "";
        }

        /**
         * @brief Run a task in the queue sychronously. This means that the task is
         * executed before the method returns.
         *
         * @param task
         */
        void syncTaskInQueue(const std::function<void()> &task)
        {
            std::promise<int> prom;
            std::future<int> fut = prom.get_future();
            runTaskInQueue([&]()
                           {
                 task();
                 prom.set_value(1); });
            fut.get();
        };
        virtual ~TaskQueue()
        {
        }
    };
}