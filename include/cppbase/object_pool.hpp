#pragma once

#ifndef CPPBASE_OBJECT_POOL_HPP
#define CPPBASE_OBJECT_POOL_HPP

#include <memory>
#include <memory_resource>

#include "nomoveable.hpp"

namespace cppbase {

/**
 * @brief Object pool for efficient memory management of type T objects
 * @details Uses std::pmr::synchronized_pool_resource for thread-safe memory pooling
 * @tparam T Type of objects to pool
 */
template <typename T>
class ObjectPool : public NoMoveable {
 public:
  using ObjectPtr = std::shared_ptr<T>;

  explicit ObjectPool(size_t max_pool = 1024 * 64)
      : memPool_(CreatePool(max_pool)) {}

  ObjectPtr Acquire() {
    void* mem = memPool_->allocate(sizeof(T), alignof(T));

    try {
      T* obj = new (mem) T();
      return ObjectPtr(obj, [this](T* ptr) { Release(ptr); });
    } catch (...) {
      memPool_->deallocate(mem, sizeof(T), alignof(T));
      throw;
    }
  }

  void Reset() { memPool_->release(); }

 private:
  std::unique_ptr<std::pmr::synchronized_pool_resource> memPool_;

  static std::unique_ptr<std::pmr::synchronized_pool_resource> CreatePool(
      size_t max_pool) {
    std::pmr::pool_options opts{};
    opts.max_blocks_per_chunk = 64;
    opts.largest_required_pool_block = sizeof(T);
    opts.max_pool_bytes = max_pool;
    return std::make_unique<std::pmr::synchronized_pool_resource>(opts);
  }

  void Release(T* obj) {
    if (!obj) {
      return;
    }

    obj->~T();
    memPool_->deallocate(obj, sizeof(T), alignof(T));
  }
};

}  // namespace cppbase

#endif  // CPPBASE_OBJECT_POOL_HPP