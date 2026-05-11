#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <chrono>

#include "cppbase/mpmc_blocking_queue.hpp"

void TestMpmcBlockingQueue() {
  std::cout << "=== Test MpmcBlockingQueue ===" << std::endl;

  cppbase::MpmcBlockingQueue<std::string> queue(10);

  auto producer = [&queue](int id) {
    for (int i = 0; i < 3; ++i) {
      std::string msg =
          "Producer" + std::to_string(id) + "_Msg" + std::to_string(i);
      queue.Enqueue(std::move(msg));
      std::cout << "Producer " << id << " enqueued: " << i << std::endl;
    }
  };

  auto consumer = [&queue](int id) {
    for (int i = 0; i < 3; ++i) {
      std::string msg;
      queue.Dequeue(msg);
      std::cout << "Consumer " << id << " dequeued: " << msg << std::endl;
    }
  };

  std::thread p1(producer, 1);
  std::thread p2(producer, 2);
  std::thread c1(consumer, 1);
  std::thread c2(consumer, 2);

  p1.join();
  p2.join();
  c1.join();
  c2.join();

  std::cout << std::endl;
}

void TestTryEnqueue() {
  std::cout << "=== Test TryEnqueue ===" << std::endl;

  cppbase::MpmcBlockingQueue<int> queue(3);

  queue.EnqueueImmediate(1);
  queue.EnqueueImmediate(2);
  queue.EnqueueImmediate(3);

  std::cout << "Queue full, try to enqueue more..." << std::endl;

  queue.TryEnqueue(4);
  queue.TryEnqueue(5);

  std::cout << "DropCount: " << queue.GetDropCount() << std::endl;
  std::cout << std::endl;
}

void TestTryDequeue() {
  std::cout << "=== Test TryDequeue ===" << std::endl;

  cppbase::MpmcBlockingQueue<int> queue(5);

  int value;

  bool success = queue.TryDequeue(value, std::chrono::milliseconds(100));
  std::cout << "TryDequeue from empty (timeout 100ms): "
            << (success ? "success" : "timeout") << std::endl;

  queue.Enqueue(42);
  success = queue.TryDequeue(value, std::chrono::milliseconds(100));
  std::cout << "TryDequeue after enqueue: " << value
            << ", success: " << success << std::endl;
  std::cout << std::endl;
}

int main() {
  TestMpmcBlockingQueue();
  TestTryEnqueue();
  TestTryDequeue();

  std::cout << "All tests passed!" << std::endl;
  return 0;
}