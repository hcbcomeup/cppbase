#include <gtest/gtest.h>
#include "cppbase/circular_queue.hpp"

TEST(CircularQueueTest, BasicPushPop)
{
    cppbase::CircularQueue<int> queue(5);

    EXPECT_TRUE(queue.IsEmpty());
    EXPECT_FALSE(queue.IsFull());

    queue.PushBack(10);
    queue.PushBack(20);
    queue.PushBack(30);
    queue.PushBack(40);

    EXPECT_EQ(queue.GetSize(), 4);
    EXPECT_FALSE(queue.IsEmpty());
    EXPECT_TRUE(queue.IsFull());

    EXPECT_EQ(queue.Front(), 10);
    queue.PopFront();
    EXPECT_EQ(queue.GetSize(), 3);
    EXPECT_FALSE(queue.IsFull());

    EXPECT_EQ(queue.Front(), 20);
    queue.PopFront();
    EXPECT_EQ(queue.GetSize(), 2);
}

TEST(CircularQueueTest, CircularOverwrite)
{
    cppbase::CircularQueue<int> queue(3);

    queue.PushBack(1);
    queue.PushBack(2);
    EXPECT_EQ(queue.GetSize(), 2);

    queue.PushBack(3);
    EXPECT_EQ(queue.GetOverRunCounter(), 1);

    EXPECT_EQ(queue.Front(), 2);
    queue.PopFront();

    EXPECT_EQ(queue.Front(), 3);
    queue.PopFront();

    EXPECT_TRUE(queue.IsEmpty());
}

TEST(CircularQueueTest, MoveConstructor)
{
    cppbase::CircularQueue<int> queue1(5);
    queue1.PushBack(10);
    queue1.PushBack(20);

    cppbase::CircularQueue<int> queue2(std::move(queue1));

    EXPECT_EQ(queue2.GetSize(), 2);
    EXPECT_EQ(queue2.Front(), 10);
}

TEST(CircularQueueTest, IsFullBehavior)
{
    cppbase::CircularQueue<int> queue(3);

    queue.PushBack(1);
    queue.PushBack(2);
    EXPECT_FALSE(queue.IsFull());

    queue.PushBack(3);
    EXPECT_TRUE(queue.IsFull());
}

TEST(CircularQueueTest, GetSizeAfterPop)
{
    cppbase::CircularQueue<int> queue(5);

    queue.PushBack(10);
    queue.PushBack(20);
    queue.PushBack(30);

    EXPECT_EQ(queue.GetSize(), 3);

    queue.PopFront();
    EXPECT_EQ(queue.GetSize(), 2);

    queue.PopFront();
    EXPECT_EQ(queue.GetSize(), 1);

    queue.PopFront();
    EXPECT_EQ(queue.GetSize(), 0);
    EXPECT_TRUE(queue.IsEmpty());
}
