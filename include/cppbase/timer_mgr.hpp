#pragma once

#ifndef CPPBASE_TIMER_MGR_HPP
#define CPPBASE_TIMER_MGR_HPP

#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <set>
#include <stdexcept>
#include <thread>
#include <vector>

#include "err_code.hpp"
#include "nocopyable.hpp"
#include "utility.hpp"

namespace cppbase {

using namespace std::chrono;
using Functor = std::function<void()>;
using TimePoint = time_point<steady_clock, steady_clock::duration>;

/**
 * @brief Timer class for scheduled task execution
 */
class Timer : public NoCopyable {
 public:
  Timer(int64_t expiration, Functor&& functor, int64_t cycleTime = 0)
      : functor_(std::move(functor)),
        cycleTime_(cycleTime),
        cycle_(cycleTime > 0) {
    if (expiration <= 0) {
      throw std::invalid_argument("expiration must > 0");
    }

    if (!functor_) {
      throw std::invalid_argument("functor is empty");
    }

    expiration_ = steady_clock::now() + nanoseconds(expiration);
  }

  TimePoint GetExpiration() const { return expiration_; }

  void SetExpiration(const TimePoint& time) { expiration_ = time; }

  int64_t GetCycleTime() const { return cycleTime_; }

  bool Cycle() const { return cycle_; }

  void Run() { functor_(); }

  friend struct TimerCompare;

 private:
  TimePoint expiration_;
  int64_t cycleTime_{0};
  bool cycle_{false};
  Functor functor_;
};

/**
 * @brief Comparator for Timer pointers (by expiration time)
 */
struct TimerCompare {
  bool operator()(const std::shared_ptr<Timer>& lhs,
                  const std::shared_ptr<Timer>& rhs) const {
    return lhs->expiration_ < rhs->expiration_;
  }
};

/**
 * @brief Timer manager for scheduled task execution
 * @details Singleton pattern, manages multiple timers in a dedicated thread
 */
class TimerMgr : public NoCopyable {
 public:
  using TimerPtr = std::shared_ptr<Timer>;

  static TimerMgr& Instance() {
    static TimerMgr inst;
    return inst;
  }

  ~TimerMgr() { Stop(); }

  void AddTimer(std::shared_ptr<Timer> timer) {
    if (!timer) {
      return;
    }

    bool need_notify = false;
    {
      std::unique_lock<std::mutex> lk(mutex_);
      timers_.insert(timer);
      need_notify = UpdateWakeUp();
    }

    if (need_notify) {
      cv_.notify_one();
    }
  }

  int DelTimer(std::shared_ptr<Timer> timer) {
    if (!timer) {
      return EC_INVALID_PARAM;
    }

    bool need_notify = false;
    {
      std::unique_lock<std::mutex> lk(mutex_);
      auto iter = timers_.find(timer);
      if (timers_.end() == iter) {
        return EC_NOT_FOUND;
      }

      timers_.erase(iter);
      need_notify = UpdateWakeUp();
    }

    if (need_notify) {
      cv_.notify_one();
    }

    return EC_OK;
  }

 private:
  bool running_{false};
  std::mutex mutex_;
  std::condition_variable cv_;
  std::thread thread_;
  std::multiset<TimerPtr, TimerCompare> timers_;
  TimePoint wakeup_;

  TimerMgr() : running_(true) {
    wakeup_ = steady_clock::now() + minutes(1);
    thread_ = std::thread([this]() { Run(); });
    Utility::SetThreadName(thread_.native_handle(), "TimerMgr");
  }

  bool UpdateWakeUp() {
    auto old = wakeup_;
    if (timers_.empty()) {
      wakeup_ = steady_clock::now() + minutes(1);
    } else {
      wakeup_ = (*timers_.begin())->GetExpiration();
    }

    return wakeup_ != old;
  }

  void Run() {
    while (running_) {
      std::unique_lock<std::mutex> lk(mutex_);
      cv_.wait_until(lk, wakeup_, [this] {
        return !running_ || !timers_.empty();
      });

      if (!running_) {
        break;
      }

      auto now = steady_clock::now();
      std::vector<TimerPtr> timers;
      GetExpired(now, timers);

      lk.unlock();

      // TODO:If the task takes too long to complete, it will affect the
      // accuracy of the timer.
      for (const auto& timer : timers) {
        try {
          timer->Run();
        } catch (...) {
          // TODO:log
        }
      }

      lk.lock();
      Reset(now, timers);
    }
  }

  void GetExpired(const TimePoint& now, std::vector<TimerPtr>& timers) {
    while (!timers_.empty()) {
      auto it = timers_.begin();
      if ((*it)->GetExpiration() > now) {
        break;
      }
      timers.push_back(*it);
      timers_.erase(it);
    }
  }

  void Reset(const TimePoint& now, const std::vector<TimerPtr>& timers) {
    for (const auto& timer : timers) {
      if (timer->Cycle()) {
        timer->SetExpiration(now + nanoseconds(timer->GetCycleTime()));
        timers_.insert(timer);
      }
    }

    bool need_notify = UpdateWakeUp();
    if (need_notify) {
      cv_.notify_one();
    }
  }

  void Stop() {
    {
      std::lock_guard<std::mutex> lk(mutex_);
      running_ = false;
    }

    cv_.notify_one();

    if (thread_.joinable()) {
      thread_.join();
    }
  }
};

}  // namespace cppbase

#endif  // CPPBASE_TIMER_MGR_HPP