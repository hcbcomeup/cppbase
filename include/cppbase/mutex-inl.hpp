#pragma once

#ifndef CPPBASE_MUTEX_INL_HPP
#define CPPBASE_MUTEX_INL_HPP

#include "mutex.hpp"

namespace cppbase {

template<typename Mtx>
void Mutex<Mtx>::lock()
{
    mutex_.lock();
}

template<typename Mtx>
bool Mutex<Mtx>::try_lock() noexcept
{
    return mutex_.try_lock();
}

template<typename Mtx>
void Mutex<Mtx>::unlock() noexcept
{
    mutex_.unlock();
}

template<typename Mtx>
template <class Rep, class Period>
auto Mutex<Mtx>::try_lock_for(const std::chrono::duration<Rep, Period>& rel_time)
    -> decltype(std::declval<Mtx>().try_lock_for(rel_time), bool())
{
    static_assert(
        std::is_same_v<Mtx, std::timed_mutex> ||
        std::is_same_v<Mtx, std::recursive_timed_mutex>,
        "Mutex<try_lock_for>: Mtx must be std::timed_mutex or std::recursive_timed_mutex!"
    );
    return mutex_.try_lock_for(rel_time);
}

template<typename Mtx>
template <class Clock, class Duration>
auto Mutex<Mtx>::try_lock_until(const std::chrono::time_point<Clock, Duration>& abs_time)
    -> decltype(std::declval<Mtx>().try_lock_until(abs_time), bool())
{
    static_assert(
        std::is_same_v<Mtx, std::timed_mutex> ||
        std::is_same_v<Mtx, std::recursive_timed_mutex>,
        "Mutex<try_lock_until>: Mtx must be std::timed_mutex or std::recursive_timed_mutex!"
    );
    return mutex_.try_lock_until(abs_time);
}

template<typename Mtx>
typename Mtx::native_handle_type Mutex<Mtx>::native_handle() noexcept
{
    return mutex_.native_handle();
}

} // namespace cppbase

#endif // CPPBASE_MUTEX_INL_HPP