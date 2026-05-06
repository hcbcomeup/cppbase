#pragma once
#ifndef CPPBASE_UTILITY_HPP
#define CPPBASE_UTILITY_HPP

#include <thread>
#include "config.hpp"

#ifdef CPPBASE_IS_LINUX
    #include "pthread.h"
#elif CPPBASE_IS_WINDOWS
    #include <process.h>
    #include <windows.h>
#endif

namespace cppbase {

using ThreadId = std::thread::native_handle_type;

class Utility
{
public:
    static void SetThreadName(ThreadId id, const std::string& name)
    {
        #ifdef CPPBASE_IS_LINUX
            pthread_setname_np(id, name.c_str());
#elif CPPBASE_IS_WINDOWS
            SetThreadDescription(id, name.c_str());
#endif
    }
};
}

#endif // CPPBASE_UTILITY_HPP