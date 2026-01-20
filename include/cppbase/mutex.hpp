#pragma once
#ifndef CPPBASE_MUTEX_HPP
#define CPPBASE_MUTEX_HPP

#include <mutex>
#include <chrono>
#include <type_traits>
#include "nocopyable.hpp"
#include "nomoveable.hpp"

namespace cppbase {

/**
 * @brief Generic template mutex class, based on standard mutex type encapsulation
 * @tparam Mtx Underlying mutex type, default is std::mutex
 * @note Template implementation is in mutex-inl.hpp, must be included after declaration
 */
template<typename Mtx = std::mutex>
class Mutex final: public NoCopyable, public NoMoveable
{
public:
    void lock();
    bool try_lock() noexcept;
    void unlock() noexcept;

    template <class Rep, class Period>
    auto try_lock_for(const std::chrono::duration<Rep, Period>& rel_time) 
        -> decltype(std::declval<Mtx>().try_lock_for(rel_time), bool());
    
    template <class Clock, class Duration>
    auto try_lock_until(const std::chrono::time_point<Clock, Duration>& abs_time)
        -> decltype(std::declval<Mtx>().try_lock_until(abs_time), bool());

    typename Mtx::native_handle_type native_handle() noexcept;
    
private:
    Mtx mutex_;
};

/**
 * @brief RAII wrapper for Mutex, auto lock/unlock
 * @tparam Mtx Underlying mutex type (same as Mutex's template parameter)
 * @note Only works with cppbase::Mutex, not std::mutex directly
 */
template<typename Mtx = std::mutex>
class LockGuard final: public NoCopyable, public NoMoveable
{
public:
    explicit LockGuard(Mutex<Mtx> &mtx) noexcept 
        : mutex_(mtx)
    {
        mutex_.lock();
    }

    ~LockGuard() noexcept
    {
        mutex_.unlock();
    }

private:
    Mutex<Mtx> &mutex_;
};

} // namespace cppbase

#include "mutex-inl.hpp"
#endif // CPPBASE_MUTEX_HPP