#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include "cppbase/threadpool.hpp"
#include "cppbase/err_code.hpp"

void TestBasicUsage()
{
    std::cout << "=== Test Basic Usage ===" << std::endl;

    cppbase::ThreadPool pool;
    int ret = pool.Start(4, 100);
    if (ret != cppbase::ErrorCode::EC_OK)
    {
        std::cout << "Failed to start thread pool, error: " << ret << std::endl;
        return;
    }

    std::cout << "Thread pool started" << std::endl;

    for (int i = 0; i < 5; ++i)
    {
        pool.AddTask([i]()
        {
            std::cout << "Task " << i << " executed by thread" << std::endl;
        });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << std::endl;
}

void TestSubmitWithFuture()
{
    std::cout << "=== Test Submit with Future ===" << std::endl;

    cppbase::ThreadPool pool;
    pool.Start(4, 100);

    auto future1 = pool.Submit([]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return 42;
    });

    auto future2 = pool.Submit([]()
    {
        return 10 + 20;
    });

    auto future3 = pool.Submit([]()
    {
        std::string msg = "Hello ThreadPool";
        return msg.length();
    });

    std::cout << "Future1 result: " << future1.get() << std::endl;
    std::cout << "Future2 result: " << future2.get() << std::endl;
    std::cout << "Future3 result: " << future3.get() << std::endl;

    std::cout << std::endl;
}

void TestAddTaskWithTimeout()
{
    std::cout << "=== Test AddTask with Timeout ===" << std::endl;

    cppbase::ThreadPool pool;
    pool.Start(1, 2);

    pool.AddTask([]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Task 1 completed" << std::endl;
    });
    pool.AddTask([]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Task 2 completed" << std::endl;
    });

    std::cout << "Queue full, trying to add with timeout..." << std::endl;

    int ret = pool.AddTask([]()
    {
        std::cout << "Task 3" << std::endl;
    }, std::chrono::milliseconds(100));

    if (ret == cppbase::ErrorCode::EC_TIMEOUT)
    {
        std::cout << "AddTask timeout" << std::endl;
    }
    else if (ret == cppbase::ErrorCode::EC_OK)
    {
        std::cout << "AddTask success" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << std::endl;
}

void TestMultiThreaded()
{
    std::cout << "=== Test Multi-threaded ===" << std::endl;

    cppbase::ThreadPool pool;
    pool.Start(4, 100);

    const int numTasks = 20;
    std::atomic<int> counter{0};

    std::vector<std::thread> threads;
    for (int t = 0; t < 4; ++t)
    {
        threads.emplace_back([&pool, &counter, t, numTasks]()
        {
            for (int i = 0; i < numTasks; ++i)
            {
                pool.AddTask([&counter, t, i]()
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    counter++;
                });
            }
        });
    }

    for (auto& th : threads)
    {
        th.join();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Expected: " << numTasks * 4 << ", Actual: " << counter << std::endl;
    std::cout << std::endl;
}

void TestExceptionHandling()
{
    std::cout << "=== Test Exception Handling ===" << std::endl;

    cppbase::ThreadPool pool;
    pool.Start(2, 10);

    auto future = pool.Submit([]()
    {
        throw std::runtime_error("Test exception");
        return 0;
    });

    try
    {
        future.get();
    }
    catch (const std::exception& e)
    {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    std::cout << std::endl;
}

int main()
{
    TestBasicUsage();
    TestSubmitWithFuture();
    TestAddTaskWithTimeout();
    TestMultiThreaded();
    TestExceptionHandling();

    std::cout << "All tests completed!" << std::endl;
    return 0;
}