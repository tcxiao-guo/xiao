/**
 * @file EpollPoller.h
 * @author xiao guo
 * @brief
 * @version 0.1
 * @date 2024-05-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "../Poller.h"

#if defined __linux__ || defined _WIN32
#include <memory>
#include <map>
using EventList = std::vector<struct epoll_event>;
#endif

namespace xiao
{
    class Channel;
    class EpollPoller : public Poller
    {
    public:
        explicit EpollPoller(EventLoop *loop);
        virtual ~EpollPoller();
        virtual void poll(int timeoutMs, ChannelList *activeChannels) override;
        virtual void updateChannel(Channel *channel) override;
        virtual void removeChannel(Channel *channel) override;

#ifdef _WIN32
        virtual void postEvent(uint64_t event) override;
        virtual void setEventCallback(const EventCallback &cb) override
        {
            eventCallback_ = cb;
        }
#endif

    private:
#if defined __linux__ || defined _WIN32
        static const int xInitEventListSize = 16;
#ifdef _WIN32
        void *epollfd_;
        EventCallback eventCallback_{[](uint64_t event) {}};
#else
        int epollfd_;
#endif
        EventList events_;
        void update(int operation, Channel *channel);
#ifndef NDEBUG
        using ChannelMap = std::map<int, Channel *>;
        ChannelMap channels_;
#endif
        void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;
#endif
    };
}