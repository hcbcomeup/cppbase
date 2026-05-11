#pragma once

#ifndef CPPBASE_UTILITY_HPP
#define CPPBASE_UTILITY_HPP

#include <string>
#include <thread>

#include "config.hpp"

#ifdef CPPBASE_IS_LINUX
#include <pthread.h>
#elif CPPBASE_IS_WINDOWS
#include <process.h>
#include <windows.h>
#endif

namespace cppbase {

using ThreadId = std::thread::native_handle_type;

/**
 * @brief Utility class for common helper functions
 */
class Utility {
 public:
  /**
   * @brief Set thread name (platform-specific)
   * @param id Native thread handle
   * @param name Name to set for the thread
   */
  static void SetThreadName(ThreadId id, const std::string& name) {
#ifdef CPPBASE_IS_LINUX
    pthread_setname_np(id, name.c_str());
#elif CPPBASE_IS_WINDOWS
    SetThreadDescription(id, name.c_str());
#endif
  }
};

}  // namespace cppbase

#endif  // CPPBASE_UTILITY_HPP