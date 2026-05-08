# cppbase

A lightweight, dependency-free, cross-platform C++17 basic library with thread pool, memory pool, message queue, timer manager and common utilities.

## Features

- **ThreadPool** - High-performance thread pool with task queue and future support
- **ObjectPool** - Memory pool for object reuse, reducing allocation overhead
- **CircularQueue** - Lock-free single-producer single-consumer circular queue
- **MpmcBlockingQueue** - Multi-producer multi-consumer blocking queue
- **TimerMgr** - Timer manager with periodic and one-shot timer support
- **Utility** - Common utilities for thread naming, etc.

## Requirements

- C++17 or higher
- CMake 3.15+
- POSIX-compatible system (Linux, macOS) or Windows

## Quick Start

### Include in your project

```cpp
#include "cppbase/threadpool.hpp"
#include "cppbase/timermgr.hpp"
#include "cppbase/objectpool.hpp"
#include "cppbase/circular_queue.hpp"
#include "cppbase/mpmc_blocking_queue.hpp"
```

### ThreadPool Example

```cpp
#include "cppbase/threadpool.hpp"

cppbase::ThreadPool pool;
pool.Start(4, 100);  // 4 threads, queue size 100

pool.AddTask([]()
{
    std::cout << "Task executed!" << std::endl;
});

// Or use Submit for future result
auto future = pool.Submit([]()
{
    return 42;
});
int result = future.get();  // result = 42
```

### TimerMgr Example

```cpp
#include "cppbase/timermgr.hpp"

auto& mgr = cppbase::TimerMgr::Instance();

// One-shot timer (1 second)
auto timer = std::make_shared<cppbase::Timer>(
    1000000000,  // 1 second in nanoseconds
    []()
    {
        std::cout << "Timer fired!" << std::endl;
    }
);
mgr.AddTimer(timer);

// Periodic timer (every 500ms)
auto periodicTimer = std::make_shared<cppbase::Timer>(
    500000000,
    []()
    {
        std::cout << "Periodic timer!" << std::endl;
    },
    500000000  // cycle time
);
mgr.AddTimer(periodicTimer);

// Delete timer
mgr.DelTimer(timer);
```

### ObjectPool Example

```cpp
#include "cppbase/objectpool.hpp"

struct MyObject
{
    int id;
    // ...
};

cppbase::ObjectPool<MyObject> pool;

// Acquire object
auto obj = pool.Acquire();
obj->id = 123;

// Release object back to pool
pool.Release(obj);
```

### CircularQueue Example

```cpp
#include "cppbase/circular_queue.hpp"

cppbase::CircularQueue<int> queue(1024);

// Producer
queue.Push(1);
queue.Push(2);

// Consumer
int value;
queue.Pop(value);  // value = 1
```

### MpmcBlockingQueue Example

```cpp
#include "cppbase/mpmc_blocking_queue.hpp"

cppbase::MpmcBlockingQueue<int> queue(1024);

// Producer thread
queue.Push(1);

// Consumer thread
int value = queue.Pop();  // blocking until data available
```

## Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

## Run Examples

```bash
./examples/threadpool_example
./examples/timermgr_example
./examples/objectpool_example
./examples/circular_queue_example
./examples/mpmc_queue_example
```

## Run Tests

```bash
ctest --output-on-failure
# Or run individual tests
./test/threadpool_test
./test/timermgr_test
./test/objectpool_test
./test/circular_queue_test
./test/mpmc_queue_test
```

## Components

| Component | Header | Description |
|-----------|--------|-------------|
| ThreadPool | `threadpool.hpp` | Thread pool with task queue and future |
| TimerMgr | `timermgr.hpp` | Timer manager with one-shot and periodic timers |
| ObjectPool | `objectpool.hpp` | Memory pool for object reuse |
| CircularQueue | `circular_queue.hpp` | Lock-free SPSC circular queue |
| MpmcBlockingQueue | `mpmc_blocking_queue.hpp` | Multi-producer multi-consumer queue |
| NoCopyable | `nocopyable.hpp` | Base class for non-copyable objects |
| NoMoveable | `nomoveable.hpp` | Base class for non-movable objects |
| Utility | `utility.hpp` | Common utilities |

## License

MIT License