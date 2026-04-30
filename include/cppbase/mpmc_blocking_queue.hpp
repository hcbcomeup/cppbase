#pragma once
#ifndef CPPBASE_MPMC_BLOCKING_QUEUE_HPP
#define CPPBASE_MPMC_BLOCKING_QUEUE_HPP

#include <condition_variable>
#include <atomic>
#include <mutex>
#include <chrono>
#include "circular_queue.hpp"
#include <cstdint>

namespace cppbase {

template<typename T>
class MpmcBlockingQueue
{
public:
    MpmcBlockingQueue(size_t size) : que_(size) {}
    ~MpmcBlockingQueue(){}

    void Enqueue(T &&item)
    {
        {
            std::unique_lock<std::mutex> lk(mutex_);
            cvPop_.wait(lk, [this]
                         { return !que_.IsFull(); });
            que_.PushBack(std::move(item));
        }

        cvPush_.notify_one();
    }

    void Enqueue_Immediate(T &&item)
    {
        {
            std::unique_lock<std::mutex> lk(mutex_);
            que_.PushBack(std::move(item));
        }

        cvPush_.notify_one();
    }

    void TryEnqueue(T &&item)
    {
        bool push = false;
        {
            std::unique_lock<std::mutex> lk(mutex_);
            if (!que_.IsFull())
            {
                push = true;
                que_.PushBack(std::move(item));
            }
        }

        if (push)
        {
            cvPush_.notify_one();
        }
        else
        {
            ++dropCounter_;
        }
    }

    /*
    @note: blocking until data coming
    */
    void DeQueue(T &item)
    {
        {
            std::unique_lock<std::mutex> lk(mutex_);
            cvPush_.wait(lk, [this]
                          { return !que_.IsEmpty(); });
            item = que_.Front();
            que_.PopFront();
        }
        cvPop_.notify_one();
    }

    bool TryDeQueue(T &item, std::chrono::milliseconds waitDuration)
    {
        {
            std::unique_lock<std::mutex> lk(mutex_);
            if (!cvPush_.wait_for(lk, waitDuration, [this]
                          { return !que_.IsEmpty(); }))
            {
                return false;
            }
            item = que_.Front();
            que_.PopFront();
        }

        cvPop_.notify_one();
        return true;
    }

    const size_t GetDropCount() const
    {
        return dropCounter_.load(std::memory_order_relaxed);
    }

    const size_t GetOverRunCounter() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return que_.GetOverRunCounter();
    }

    const size_t GetSize() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return que_.GetSize();
    }

private:
    mutable std::mutex mutex_;
    CircularQueue<T> que_;
    std::condition_variable cvPush_;
    std::condition_variable cvPop_;
    std::atomic<size_t> dropCounter_ = 0;
};

} // namespace cppbase

#endif // CPPBASE_MPMC_BLOCKING_QUEUE_HPP