#pragma once
#ifndef CPPBASE_OBJECT_POOL_HPP
#define CPPBASE_OBJECT_POOL_HPP

#include <memory_resource>
#include <memory>
#include "nomoveable.hpp"

namespace cppbase {

template<typename T>
class ObjectPool : public NoMoveable
{
public:
    using ObjectPtr = std::shared_ptr<T>;

    explicit ObjectPool(size_t max_pool = 1024 * 64)
    {
        std::pmr::pool_options opts{};
        opts.max_blocks_per_chunk = 64;
        opts.largest_required_pool_block = sizeof(T);
        opts.max_pool_bytes = max_pool;

        memPool_ = std::make_unique<std::pmr::synchronized_pool_resource>(opts);
    }

    ObjectPtr Acquire()
    {
        void *mem = memPool_->allocate(sizeof(T), alignof(T));

        try
        {
            T *obj = new(mem) T();
            return ObjectPtr(obj, [this](T *ptr)
                             { Release(ptr); });
        }
        catch(...)
        {
            memPool_->deallocate(mem, sizeof(T), alignof(T));
            throw;
        }
    }

    void Reset()
    {
        memPool_->release();
    }

private:
    std::unique_ptr<std::pmr::synchronized_pool_resource> memPool_;

private:
    void Release(T *obj)
    {
        if (!obj)
        {
            return;
        }

        obj->~T();

        memPool_->deallocate(obj, sizeof(T), alignof(T));
    }
};
}

#endif // CPPBASE_OBJECT_POOL_HPP