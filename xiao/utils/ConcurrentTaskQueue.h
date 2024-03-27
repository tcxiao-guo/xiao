/**
 * @file ConcurrentTaskQueue.h
 * @author Xiao Guo
 * @brief
 * @version 0.1
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <xiao/utils/TaskQueue.h>
#include <xiao/exports.h>
#include <queue>

namespace xiao
{
    /**
     * @brief This class implements a task queue running in parallel. Basically this
     * can be called a threads pool.
     *
     */
    class XIAO_EXPORT ConcurrentTaskQueue : public TaskQueue
    {
    public:
        /**
         * @brief Construct a new concurrent task queue instance.
         *
         * @param threadNum The number of threads in the queue.
         * @param name The name of the queue.
         */
        ConcurrentTaskQueue(size_t threadNum, const std::string &name);

        /**
         * @brief Run a task in the queue.
         *
         * @param task
         */
        virtual void runTaskInQueue(const std::function<void()> &task);
        virtual void runTaskInQueue(std::function<void()> &&task);

        /**
         * @brief Get the name of the queue.
         *
         * @return std::string
         */
        virtual std::string getName() const
        {
            return queueName_;
        };

        /**
         * @brief Get the number of tasks to be executed in the queue.
         *
         * @return size_t
         */
        size_t getTaskCount();

        /**
         * @brief Stop all threads in the queue.
         *
         */
        void stop();

        ~ConcurrentTaskQueue();

    private:
        size_t queueCount_;
        std::string queueName_;

        std::queue<std::function<void()>> taskQueue_;
        std::vector<std::thread> threads_;

        std::mutex taskMutex_;
        std::condition_variable taskCond_;
        std::atomic_bool stop_;
        void queueFunc(int queueNum);
    };
}