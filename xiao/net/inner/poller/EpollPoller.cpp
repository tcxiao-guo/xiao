/**
 * @file EpollPoller.cpp
 * @author xiao guo
 * @brief
 * @version 0.1
 * @date 2024-05-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "EpollPoller.h"
#include <xiao/utils/Logger.h>

#ifdef __linux__

#elif defined _WIN32
#include "Wepoll.h"
#endif

namespace xiao
{
#if defined __linux__ || defined _WIN32

#if defined __linux__
    static_assert(EPOLLIN == POLLIN, "EPOLLIN != POLLIN");
    static_assert(EPOLLPRI == POLLPRI, "EPOLLPRI != POLLPRI");
    static_assert(EPOLLOUT == POLLOUT, "EPOLLOUT != POLLOUT");
    static_assert(EPOLLRDHUP == POLLRDHUP, "EPOLLRDHUP != POLLRDHUP");
    static_assert(EPOLLERR == POLLERR, "EPOLLERR != POLLERR");
    static_assert(EPOLLHUP == POLLHUP, "EPOLLHUP != POLLHUP");
#endif

    namespace
    {
        const int xNew = -1;
        const int xAdded = 1;
        const int xDeleted = 2;
    }

    EpollPoller::EpollPoller(EventLoop *loop)
        : Poller(loop),
#ifdef _WIN32
          // wepoll does not suppor flags
          epollfd_(::epoll_create1(0)),
#else
          epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
#endif
          events_(xInitEventListSize)
    {
    }
    EpollPoller::~EpollPoller()
    {
#ifdef _WIN32
        epoll_close(epollfd_);
#else
        close(epollfd_);
#endif
    }
#ifdef _WIN32
    void EpollPoller::postEvent(uint64_t event)
    {
        epoll_post_signal(epollfd_, event);
    }
#endif
    void EpollPoller::poll(int timeoutMs, ChannelList *activeChannels)
    {
        int numEvents = ::epoll_wait(epollfd_,
                                     &*events_.begin(),
                                     static_cast<int>(events_.size()),
                                     timeoutMs);
        int savedErrno = errno;

        if (numEvents > 0)
        {
            fillActiveChannels(numEvents, activeChannels);
            if (static_cast<size_t>(numEvents) == events_.size())
            {
                events_.resize(events_.size() * 2);
            }
        }
        else if (numEvents == 0)
        {
            // std::cout << "nothing happended" << std::endl;
        }
        else
        {
            if (savedErrno != EINTR)
            {
                errno = savedErrno;
                LOG_SY
            }
        }
    }
#else
#endif
#endif
} // namespace xiao
