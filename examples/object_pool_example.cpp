#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <memory>

#include "cppbase/object_pool.hpp"

struct Connection {
  int id;
  std::string name;

  Connection() : id(0), name("default") {
    std::cout << " [Connection] Constructed (id=" << id << ")\n";
  }

  explicit Connection(int i) : id(i), name("conn-" + std::to_string(i)) {
    std::cout << "  [Connection] Constructed (id=" << id << ", name=" << name
              << ")\n";
  }

  ~Connection() {
    std::cout << "  [Connection] Destructed (id=" << id << ")\n";
  }

  void Process(const std::string& task) {
    std::cout << "  [Connection " << id << "] Processing: " << task << "\n";
  }
};

int main() {
  std::cout << "=== ObjectPool Example ===\n\n";

  cppbase::ObjectPool<Connection> pool(1024);

  std::cout << "1. Acquire a connection from pool:\n";
  auto conn1 = pool.Acquire();
  conn1->id = 1;
  conn1->Process("SELECT * FROM users");

  std::cout << "\n2. Acquire another connection:\n";
  auto conn2 = pool.Acquire();
  conn2->id = 2;
  conn2->Process("INSERT INTO logs VALUES (...)");

  std::cout
      << "\n3. Release connections back to pool (going out of scope):\n";
  conn1.reset();
  conn2.reset();

  std::cout << "\n4. Acquire connections again (reusing from pool):\n";
  auto conn3 = pool.Acquire();
  conn3->id = 3;
  conn3->Process("UPDATE users SET status=1");

  auto conn4 = pool.Acquire();
  conn4->id = 4;
  conn4->Process("DELETE FROM cache");

  std::cout << "\n5. Multi-threaded test:\n";
  std::vector<std::thread> threads;
  for (int i = 0; i < 3; ++i) {
    threads.emplace_back([&pool, i]() {
      auto conn = pool.Acquire();
      conn->id = 100 + i;
      conn->Process("Thread-" + std::to_string(i) + " task");
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    });
  }

  for (auto& th : threads) {
    th.join();
  }

  std::cout << "\n6. Reset pool:\n";
  pool.Reset();

  std::cout << "\n=== Example Complete ===\n";

  return 0;
}