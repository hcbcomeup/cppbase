#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <stdexcept>
#include <vector>

#include "cppbase/thread_pool.hpp"
#include "cppbase/err_code.hpp"

TEST(ThreadPoolTest, BasicStart) {
  cppbase::ThreadPool pool;

  int ret = pool.Start(4, 100);
  EXPECT_EQ(ret, cppbase::ErrorCode::EC_OK);

  ret = pool.Start(2, 50);
  EXPECT_EQ(ret, cppbase::ErrorCode::EC_RUNNING);
}

TEST(ThreadPoolTest, AddTask) {
  cppbase::ThreadPool pool;
  pool.Start(2, 10);

  std::atomic<int> counter{0};

  for (int i = 0; i < 5; ++i) {
    pool.AddTask([&counter]() { counter++; });
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  EXPECT_EQ(counter, 5);
}

TEST(ThreadPoolTest, AddTaskWithTimeout) {
  cppbase::ThreadPool pool;
  pool.Start(1, 1);

  pool.AddTask([]() { std::this_thread::sleep_for(std::chrono::milliseconds(200)); });

  int ret = pool.AddTask([]() {}, std::chrono::milliseconds(50));

  EXPECT_EQ(ret, cppbase::ErrorCode::EC_TIMEOUT);
}

TEST(ThreadPoolTest, SubmitWithFuture) {
  cppbase::ThreadPool pool;
  pool.Start(2, 10);

  auto future1 = pool.Submit([]() { return 42; });

  auto future2 = pool.Submit([]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return 100;
  });

  EXPECT_EQ(future1.get(), 42);
  EXPECT_EQ(future2.get(), 100);
}

TEST(ThreadPoolTest, SubmitWithException) {
  cppbase::ThreadPool pool;
  pool.Start(2, 10);

  auto future = pool.Submit([]() {
    throw std::runtime_error("Test exception");
    return 0;
  });

  EXPECT_THROW(future.get(), std::runtime_error);
}

TEST(ThreadPoolTest, MultiThreaded) {
  cppbase::ThreadPool pool;
  pool.Start(4, 100);

  const int numTasks = 50;
  std::atomic<int> counter{0};

  std::vector<std::thread> threads;
  for (int t = 0; t < 4; ++t) {
    threads.emplace_back([&pool, &counter, numTasks]() {
      for (int i = 0; i < numTasks; ++i) {
        pool.AddTask([&counter]() { counter++; });
      }
    });
  }

  for (auto& th : threads) {
    th.join();
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  EXPECT_EQ(counter, numTasks * 4);
}

TEST(ThreadPoolTest, Stop) {
  cppbase::ThreadPool pool;
  pool.Start(2, 10);

  pool.Stop();

  int ret = pool.AddTask([]() {});

  EXPECT_EQ(ret, cppbase::ErrorCode::EC_STOPPED);
}