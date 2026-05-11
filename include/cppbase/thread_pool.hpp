#pragma once

#ifndef CPPBASE_THREAD_POOL_HPP
#define CPPBASE_THREAD_POOL_HPP

#include <chrono>
#include <future>
#include <memory>
#include <mutex>
#include <functional>
#include <vector>

#include "circular_queue.hpp"
#include "err_code.hpp"
#include "utility.hpp"

namespace cppbase {

using Task = std::function<void()>;

/**
 * @brief Thread pool for parallel task execution
 * @details Multi-threaded task queue with blocking and non-blocking task
 * submission
 */
class ThreadPool : public NoCopyable {
 public:
  ThreadPool() = default;
  ~ThreadPool() { Stop(); }

  int Start(size_t threadNum, size_t taskCapacity) {
    if (0 == threadNum || 0 == taskCapacity) {
      return ErrorCode::EC_INVALID_PARAM;
    }

    if (running_) {
      return ErrorCode::EC_RUNNING;
    }

    threadNum_ = threadNum;
    taskCapacity_ = taskCapacity;
    que_ = std::make_unique<CircularQueue<Task>>(taskCapacity_);

    running_ = true;

    std::string threadName("ThreadPool_");
    for (size_t i = 0; i < threadNum; ++i) {
      std::thread thread([this] { Run(); });
      std::string name = threadName + std::to_string(i);
      Utility::SetThreadName(thread.native_handle(), name);
      works_.emplace_back(std::move(thread));
    }

    return ErrorCode::EC_OK;
  }

  int AddTask(Task&& task) {
    std::unique_lock<std::mutex> lk(mutex_);
    cvNotFull_.wait(lk, [this] { return !running_ || !que_->IsFull(); });

    if (!running_) {
      return ErrorCode::EC_STOPPED;
    }

    que_->PushBack(std::move(task));
    cvNotEmpty_.notify_all();
    return ErrorCode::EC_OK;
  }

  int AddTask(Task&& task, std::chrono::milliseconds waitTime) {
    std::unique_lock<std::mutex> lk(mutex_);
    auto status = cvNotFull_.wait_for(lk, waitTime,
                                      [this] { return !running_ || !que_->IsFull(); });

    if (!running_) {
      return ErrorCode::EC_STOPPED;
    }

    if (!status)  // timeout returns false, no_timeout returns true
    {
      return ErrorCode::EC_TIMEOUT;
    }

    que_->PushBack(std::move(task));
    cvNotEmpty_.notify_all();
    return ErrorCode::EC_OK;
  }

  template <typename F>
  std::future<std::result_of_t<F()>> Submit(F&& task) {
    using ReturnType = std::result_of_t<F()>;
    using PackagedTask = std::packaged_task<ReturnType()>;

    auto packagedTask = std::make_shared<PackagedTask>(
        [task = std::forward<F>(task)]() { return task(); });

    std::future<ReturnType> future = packagedTask->get_future();

    AddTask([packagedTask]() { (*packagedTask)(); });

    return future;
  }

  void Stop() {
    {
      std::unique_lock<std::mutex> lk(mutex_);
      running_ = false;
    }

    cvNotFull_.notify_all();
    cvNotEmpty_.notify_all();

    for (size_t i = 0; i < threadNum_; ++i) {
      if (works_[i].joinable()) {
        works_[i].join();
      }
    }
  }

 private:
  bool running_{false};
  size_t threadNum_{0};
  size_t taskCapacity_{0};
  std::vector<std::thread> works_;
  mutable std::mutex mutex_;
  std::condition_variable cvNotEmpty_;
  std::condition_variable cvNotFull_;
  std::unique_ptr<CircularQueue<Task>> que_;

  void Run() {
    while (running_) {
      {
        std::unique_lock<std::mutex> lk(mutex_);
        cvNotEmpty_.wait(lk, [this] { return !running_ || !que_->IsEmpty(); });
        if (!running_) {
          break;
        }

        auto task = que_->Front();
        que_->PopFront();

        try {
          task();
        } catch (const std::exception& e) {
          // TODO: handle exception
        }
      }

      cvNotFull_.notify_all();
    }
  }
};

}  // namespace cppbase

#endif  // CPPBASE_THREAD_POOL_HPP