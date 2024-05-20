/**
 * @file Channel.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-03-30
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <xiao/utils/NonCopyable.h>
#include <functional>
#include <memory>

namespace xiao
{
    class EventLoop;
    /**
     * @brief This class is used to implement reactor pattern. A Channel object
     * manages a socket fd. Users use a Channel object to receive write or read
     * events on the socket it manages.
     *
     */
    class XIAO_EXPORT Channel : NonCopyable
    {
    public:
        using EventCallback = std::function<void()>;
        /**
         * @brief Construct a new Channel instance.
         *
         * @param loop The event loop in which the channel works.
         * @param fd The socket fd.
         */
        Channel(EventLoop *loop, int fd);

        /**
         * @brief Set the read callback.
         *
         * @param cb The callback is called when read enent occurs on the socket.
         * @note One should call the enableReading() method to ensure that the
         * callback would be called when some data is received on the socket.
         */
        void setReadCallback(const EventCallback &cb)
        {
            readCallback_ = cb;
        };
        void setReadCallback(EventCallback &&cb)
        {
            readCallback_ = std::move(cb);
        }

        /**
         * @brief Set the write callback.
         *
         */
        void setWriteCallback(const EventCallback &cb)
        {
            writeCallback_ = cb;
        };
        void setWriteCallback(EventCallback &&cb)
        {
            writeCallback_ = std::move(cb);
        }

        /**
         * @brief Set the close callback.
         *
         * @param cb The callback is called when the socket is closed.
         */
        void setCloseCallback(const EventCallback &cb)
        {
            closeCallback_ = cb;
        }
        void setCloseCallback(EventCallback &&cb)
        {
            closeCallback_ = std::move(cb);
        }

        /**
         * @brief Set the error callback.
         *
         * @param cb The callback is called when an error occurs on the socket.
         */
        void setErrorCallback(const EventCallback &cb)
        {
            errorCallback_ = cb;
        }
        void setErrorCallback(EventCallback &&cb)
        {
            errorCallback_ = std::move(cb);
        }

        /**
         * @brief Set the event callback.
         *
         * @param cb The callback is called when any event occurs on the socket.
         * @note If the event callback is set to the channel, any other callback
         * wouldn't be called again.
         */
        void setEventCallback(const EventCallback &cb)
        {
            eventCallback_ = cb;
        }
        void setEventCallback(EventCallback &&cb)
        {
            eventCallback_ = std::move(cb);
        }

        /**
         * @brief Return the fd of the socket.
         *
         * @return int
         */
        int fd() const
        {
            return fd_;
        }

        /**
         * @brief Return the events enabled on the socket.
         *
         * @return int
         */
        int events() const
        {
            return events_;
        }

        /**
         * @brief Return the events that occurred on the socket.
         *
         * @return int
         */
        int revents() const
        {
            return revents_;
        }

        /**
         * @brief Check whether there is no event enabled on the socket.
         *
         * @return true
         * @return false
         */
        bool isNoneEvent() const
        {
            return events_ == xNoneEvent;
        }

        /**
         * @brief Disable all events on the socket.
         *
         */
        void disableAll()
        {
            events_ = xNoneEvent;
            update();
        }

        /**
         * @brief Remove the socket from the poller in the event loop.
         *
         */
        void remove();

        /**
         * @brief Return the event loop.
         *
         * @return EventLoop*
         */
        EventLoop *ownerLoop()
        {
            return loop_;
        };

        /**
         * @brief Enable the read event on the socket.
         *
         */
        void enableReading()
        {
            events_ |= xReadEvent;
            update();
        }

        /**
         * @brief Disable the read event on the socket.
         *
         */
        void disableReading()
        {
            events_ &= ~xReadEvent;
            update();
        }

        /**
         * @brief Enable the write event on the socket.
         *
         */
        void enableWriting()
        {
            events_ |= xWriteEvent;
            update();
        }

        /**
         * @brief Disable the write event on the socket.
         *
         */
        void disableWriting()
        {
            events_ &= ~xWriteEvent;
            update();
        }

        /**
         * @brief Check whether the write event is enabled on the socket.
         *
         * @return true
         * @return false
         */
        bool isWriting() const
        {
            return events_ & xWriteEvent;
        }

        /**
         * @brief Check whether the read event is enabled on the socket.
         *
         * @return true
         * @return false
         */
        bool isReading() const
        {
            return events_ & xReadEvent;
        }

        /**
         * @brief Set and update the events enabled.
         *
         * @param events
         */
        void updateEvents(int events)
        {
            events_ = events;
            update();
        }

        /**
         * @brief This mehod is used to ensure that the callback owner is valid
         * when a callback is called.
         *
         * @param obj The callback owner. Usually, the owner is also the owner of
         * the channel/
         * @note The 'obj' is kept in a weak_ptr object, so this method does not
         * cause a circular reference problem.
         */
        void tie(const std::shared_ptr<void> &obj)
        {
            tie_ = obj;
            tied_ = true;
        }

        static const int xNoneEvent;
        static const int xReadEvent;
        static const int xWriteEvent;

    private:
        void update();
        EventLoop *loop_;
        EventCallback readCallback_;
        EventCallback writeCallback_;
        EventCallback closeCallback_;
        EventCallback errorCallback_;
        EventCallback eventCallback_;
        const int fd_;
        int events_;
        int revents_;
        int index_;
        std::weak_ptr<void> tie_;
        bool tied_;
    };

} // namespace xiao
