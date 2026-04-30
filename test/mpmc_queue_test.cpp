#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <string>
#include <atomic>
#include <chrono>
#include "cppbase/mpmc_blocking_queue.hpp"

TEST(MpmcBlockingQueueTest, BasicEnqueueDequeue)
{
    cppbase::MpmcBlockingQueue<int> queue(10);

    EXPECT_EQ(queue.GetSize(), 0);

    queue.Enqueue(1);
    queue.Enqueue(2);
    queue.Enqueue(3);

    EXPECT_EQ(queue.GetSize(), 3);

    int value;
    queue.DeQueue(value);
    EXPECT_EQ(value, 1);

    queue.DeQueue(value);
    EXPECT_EQ(value, 2);

    queue.DeQueue(value);
    EXPECT_EQ(value, 3);
}

TEST(MpmcBlockingQueueTest, TryEnqueue)
{
    cppbase::MpmcBlockingQueue<int> queue(3);

    queue.Enqueue_Immediate(1);
    queue.Enqueue_Immediate(2);
    EXPECT_EQ(queue.GetSize(), 2);

    queue.TryEnqueue(3);
    queue.TryEnqueue(4);

    EXPECT_EQ(queue.GetDropCount(), 2);
}

TEST(MpmcBlockingQueueTest, TryDeQueueTimeout)
{
    cppbase::MpmcBlockingQueue<int> queue(5);

    int value;
    bool success = queue.TryDeQueue(value, std::chrono::milliseconds(100));
    EXPECT_FALSE(success);

    queue.Enqueue(42);
    success = queue.TryDeQueue(value, std::chrono::milliseconds(100));
    EXPECT_TRUE(success);
    EXPECT_EQ(value, 42);
}

TEST(MpmcBlockingQueueTest, MultiThreaded)
{
    cppbase::MpmcBlockingQueue<std::string> queue(100);

    const int numProducers = 3;
    const int numMessagesPerProducer = 10;
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    std::atomic<int> consumedCount{0};

    for (int i = 0; i < numProducers; ++i)
    {
        producers.emplace_back([&queue, i, numMessagesPerProducer]()
        {
            for (int j = 0; j < numMessagesPerProducer; ++j)
            {
                std::string msg = "P" + std::to_string(i) + "_M" + std::to_string(j);
                queue.Enqueue(std::move(msg));
            }
        });
    }

    for (int i = 0; i < numProducers; ++i)
    {
        consumers.emplace_back([&queue, &consumedCount, numMessagesPerProducer]()
        {
            for (int j = 0; j < numMessagesPerProducer; ++j)
            {
                std::string msg;
                queue.DeQueue(msg);
                consumedCount++;
            }
        });
    }

    for (auto& t : producers)
    {
        t.join();
    }
    for (auto& t : consumers)
    {
        t.join();
    }

    EXPECT_EQ(consumedCount, numProducers * numMessagesPerProducer);
}

TEST(MpmcBlockingQueueTest, GetOverRunCounter)
{
    cppbase::MpmcBlockingQueue<int> queue(3);

    queue.Enqueue_Immediate(1);
    queue.Enqueue_Immediate(2);
    queue.Enqueue_Immediate(3);
    queue.Enqueue_Immediate(4);

    EXPECT_GE(queue.GetOverRunCounter(), 1);
}

TEST(MpmcBlockingQueueTest, EnqueueImmediate)
{
    cppbase::MpmcBlockingQueue<int> queue(2);

    queue.Enqueue_Immediate(1);
    queue.Enqueue_Immediate(2);

    int value;
    queue.DeQueue(value);
    EXPECT_EQ(value, 1);

    queue.DeQueue(value);
    EXPECT_EQ(value, 2);
}