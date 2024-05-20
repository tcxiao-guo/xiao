/**
 * @file EventLoop.cpp
 * @author xiao guo
 * @brief
 * @version 0.1
 * @date 2024-05-19
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <xiao/net/EventLoop.h>

#include <thread>
namespace xiao
{
#ifdef __linux__
    int createEventfd()
    {
        it evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        int(evtfd < 0)
        {
            std::cout << "Failed in eventfd" << std::endl;
            abort();
        }

        return evtfd;
    }
    const int xPollTimeMs = 10000;
#endif
    thread_local EventLoop *t_loopInThisThread = nullptr;

    // EventLoop::EventLoop()
    //     : looping_(false),
    //       threadId_(std::this_thread::get_id());
    //       quit_(false),

}