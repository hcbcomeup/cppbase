#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include "cppbase/mpmc_blocking_queue.hpp"

void TestMpmcBlockingQueue()
{
    std::cout << "=== Test MpmcBlockingQueue ===" << std::endl;

    cppbase::MpmcBlockingQueue<std::string> queue(10);

    auto producer = [&queue](int id)
    {
        for (int i = 0; i < 3; ++i)
        {
            std::string msg = "Producer" + std::to_string(id) + "_Msg" + std::to_string(i);
            queue.Enqueue(std::move(msg));
            std::cout << "Producer " << id << " enqueued: " << i << std::endl;
        }
    };

    auto consumer = [&queue](int id)
    {
        for (int i = 0; i < 3; ++i)
        {
            std::string msg;
            queue.DeQueue(msg);
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

void TestTryEnqueue()
{
    std::cout << "=== Test TryEnqueue ===" << std::endl;

    cppbase::MpmcBlockingQueue<int> queue(3);

    queue.Enqueue_Immediate(1);
    queue.Enqueue_Immediate(2);
    queue.Enqueue_Immediate(3);

    std::cout << "Queue full, try to enqueue more..." << std::endl;

    queue.TryEnqueue(4);
    queue.TryEnqueue(5);

    std::cout << "DropCount: " << queue.GetDropCount() << std::endl;
    std::cout << std::endl;
}

void TestTryDeQueue()
{
    std::cout << "=== Test TryDeQueue ===" << std::endl;

    cppbase::MpmcBlockingQueue<int> queue(5);

    int value;

    bool success = queue.TryDeQueue(value, std::chrono::milliseconds(100));
    std::cout << "TryDeQueue from empty (timeout 100ms): " << (success ? "success" : "timeout") << std::endl;

    queue.Enqueue(42);
    success = queue.TryDeQueue(value, std::chrono::milliseconds(100));
    std::cout << "TryDeQueue after enqueue: " << value << ", success: " << success << std::endl;
    std::cout << std::endl;
}

int main()
{
    TestMpmcBlockingQueue();
    TestTryEnqueue();
    TestTryDeQueue();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
