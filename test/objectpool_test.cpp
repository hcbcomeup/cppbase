#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <stdexcept>
#include "cppbase/objectpool.hpp"

struct TestObject
{
    int value;
    static int construct_count;
    static int destruct_count;

    TestObject() : value(0)
    {
        ++construct_count;
    }

    explicit TestObject(int v) : value(v)
    {
        ++construct_count;
    }

    ~TestObject()
    {
        ++destruct_count;
    }

    static void ResetCounters()
    {
        construct_count = 0;
        destruct_count = 0;
    }
};

int TestObject::construct_count = 0;
int TestObject::destruct_count = 0;

TEST(ObjectPoolTest, BasicAcquire)
{
    cppbase::ObjectPool<TestObject> pool;
    TestObject::ResetCounters();

    auto obj = pool.Acquire();

    EXPECT_NE(obj, nullptr);
    EXPECT_EQ(TestObject::construct_count, 1);
    EXPECT_EQ(TestObject::destruct_count, 0);
}

TEST(ObjectPoolTest, AcquireWithValue)
{
    cppbase::ObjectPool<TestObject> pool(1024);

    auto obj = pool.Acquire();
    obj->value = 42;

    EXPECT_EQ(obj->value, 42);
}

TEST(ObjectPoolTest, ObjectReuse)
{
    cppbase::ObjectPool<TestObject> pool;
    TestObject::ResetCounters();

    {
        auto obj1 = pool.Acquire();
        obj1->value = 100;
    }

    EXPECT_EQ(TestObject::destruct_count, 1);

    {
        auto obj2 = pool.Acquire();
        EXPECT_EQ(obj2->value, 0);
    }

    EXPECT_EQ(TestObject::construct_count, 2);
    EXPECT_EQ(TestObject::destruct_count, 1);
}

TEST(ObjectPoolTest, MultipleAcquire)
{
    cppbase::ObjectPool<TestObject> pool;
    TestObject::ResetCounters();

    std::vector<cppbase::ObjectPool<TestObject>::ObjectPtr> objects;

    for (int i = 0; i < 10; ++i)
    {
        objects.push_back(pool.Acquire());
    }

    EXPECT_EQ(TestObject::construct_count, 10);
    EXPECT_EQ(TestObject::destruct_count, 0);

    objects.clear();

    EXPECT_EQ(TestObject::destruct_count, 10);
}

TEST(ObjectPoolTest, MultiThreadedAcquire)
{
    cppbase::ObjectPool<TestObject> pool;
    TestObject::ResetCounters();

    const int numThreads = 4;
    const int objectsPerThread = 50;
    std::atomic<int> counter{0};

    std::vector<std::thread> threads;
    for (int t = 0; t < numThreads; ++t)
    {
        threads.emplace_back([&pool, &counter, objectsPerThread]()
        {
            for (int i = 0; i < objectsPerThread; ++i)
            {
                auto obj = pool.Acquire();
                obj->value = i;
                counter++;
            }
        });
    }

    for (auto& th : threads)
    {
        th.join();
    }

    EXPECT_EQ(counter, numThreads * objectsPerThread);
    EXPECT_EQ(TestObject::construct_count, numThreads * objectsPerThread);
}

TEST(ObjectPoolTest, Reset)
{
    cppbase::ObjectPool<TestObject> pool;
    TestObject::ResetCounters();

    {
        auto obj1 = pool.Acquire();
        auto obj2 = pool.Acquire();
    }

    EXPECT_EQ(TestObject::destruct_count, 2);

    pool.Reset();

    EXPECT_EQ(TestObject::destruct_count, 2);
}

TEST(ObjectPoolTest, CustomConstructor)
{
    cppbase::ObjectPool<TestObject> pool;
    TestObject::ResetCounters();

    auto obj = pool.Acquire();
    EXPECT_EQ(obj->value, 0);

    obj->value = 123;
    EXPECT_EQ(obj->value, 123);
}

TEST(ObjectPoolTest, NullptrRelease)
{
    cppbase::ObjectPool<TestObject> pool;
    TestObject::ResetCounters();

    auto obj = pool.Acquire();
    EXPECT_NE(obj, nullptr);

    obj.reset();

    EXPECT_EQ(obj, nullptr);
    EXPECT_EQ(TestObject::destruct_count, 1);
}