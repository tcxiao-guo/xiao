/**
 * @file Poller.cc
 * @author xiao guo
 * @brief
 * @version 0.1
 * @date 2024-05-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Poller.h"
#ifdef __linux__
#elif defined _WIN32
#include "Wepoll.h"
#elif
#else
#endif