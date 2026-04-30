#pragma once
#ifndef CPPBASE_CIRCULAR_QUEUE_HPP
#define CPPBASE_CIRCULAR_QUEUE_HPP

#include <vector>
#include <cstdint>
#include "nocopyable.hpp"

namespace cppbase {

template<typename T>
class CircularQueue : public NoCopyable
{
public:
    CircularQueue() = default;
    CircularQueue(size_t cap) : capacity_(cap),
                                que_(capacity_){}

    CircularQueue(CircularQueue &&rhs) noexcept
    {
        MoveAble(std::move(rhs));
    }

    CircularQueue &operator=(CircularQueue &&rhs) noexcept
    {
        MoveAble(std::move(rhs));
        return *this;
    }

    void PushBack(T &&item)
    {
        if (capacity_ > 0)
        {
            que_[tail_] = std::move(item);
            tail_ = (tail_ + 1) % capacity_;

            if (head_ == tail_)
            {
                head_ = (head_ + 1) % capacity_;
                ++overRunCounter_;
            }
        }
    }

    const T& Front() const
    {
        return que_[head_];
    }

    T& Front()
    {
        return que_[head_];
    }

    void PopFront()
    {
        head_ = (head_ + 1) % capacity_;
    }

    const size_t GetSize() const
    {
        if (IsEmpty())
        {
            return 0;
        }
        if (tail_ >= head_)
        {
            return tail_ - head_;
        }
        else
        {
            return capacity_ - (head_ - tail_);
        }
    }

    bool IsEmpty() const
    {
        return head_ == tail_;
    }

    bool IsFull() const
    {
        if (capacity_ > 0)
        {
            return (tail_ + 1) % capacity_ == head_;
        }

        return false;
    }

    const size_t GetOverRunCounter() const
    {
        return overRunCounter_;
    }

private:
    size_t capacity_ = 0;
    size_t overRunCounter_ = 0;
    typename std::vector<T>::size_type head_ = 0;
    typename std::vector<T>::size_type tail_ = 0;
    std::vector<T> que_;

private:
    void MoveAble(CircularQueue &&rhs)
    {
        capacity_ = rhs.capacity_;
        head_ = rhs.head_;
        tail_ = rhs.tail_;
        que_ = std::move(rhs.que_);

        rhs.capacity_ = 0;
        rhs.head_ = 0;
        rhs.tail_ = 0;
    }
};

} // namespace cppbase

#endif // CPPBASE_CIRCULAR_QUEUE_HPP