#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <memory>
#include "cppbase/timermgr.hpp"
#include "cppbase/err_code.hpp"

class TimerMgrTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(TimerMgrTest, AddTimer_Basic)
{
    auto& mgr = cppbase::TimerMgr::Instance();
    std::atomic<bool> executed{false};

    auto timer = std::make_shared<cppbase::Timer>(
        100000000, [&executed](){
            executed = true;
        }
    );

    mgr.AddTimer(timer);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    EXPECT_TRUE(executed);
}

TEST_F(TimerMgrTest, AddTimer_Multiple)
{
    auto& mgr = cppbase::TimerMgr::Instance();
    std::atomic<int> counter{0};

    for (int i = 0; i < 5; ++i)
    {
        auto timer = std::make_shared<cppbase::Timer>(
            (i + 1) * 100000000, [&counter](){
                counter++;
            }
        );
        mgr.AddTimer(timer);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(600));

    EXPECT_EQ(counter, 5);
}

TEST_F(TimerMgrTest, PeriodicTimer)
{
    auto& mgr = cppbase::TimerMgr::Instance();
    std::atomic<int> counter{0};

    auto timer = std::make_shared<cppbase::Timer>(
        100000000, [&counter](){
            counter++;
        }, 100000000
    );

    mgr.AddTimer(timer);
    std::this_thread::sleep_for(std::chrono::milliseconds(350));

    EXPECT_GE(counter, 3);
    EXPECT_LE(counter, 4);
}

TEST_F(TimerMgrTest, DelTimer_Basic)
{
    auto& mgr = cppbase::TimerMgr::Instance();
    std::atomic<int> counter{0};

    auto timer = std::make_shared<cppbase::Timer>(
        100000000, [&counter](){
            counter++;
        }, 100000000
    );

    mgr.AddTimer(timer);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    int ret = mgr.DelTimer(timer);
    EXPECT_EQ(ret, cppbase::EC_OK);

    int oldCounter = counter;
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    EXPECT_EQ(counter, oldCounter);
}

TEST_F(TimerMgrTest, DelTimer_NotFound)
{
    auto& mgr = cppbase::TimerMgr::Instance();

    auto timer = std::make_shared<cppbase::Timer>(
        1000000000, [](){}
    );

    int ret = mgr.DelTimer(timer);
    EXPECT_EQ(ret, cppbase::EC_NOT_FOUND);
}

TEST_F(TimerMgrTest, DelTimer_Nullptr)
{
    auto& mgr = cppbase::TimerMgr::Instance();

    int ret = mgr.DelTimer(nullptr);
    EXPECT_EQ(ret, cppbase::EC_INVALID_PARAM);
}

TEST_F(TimerMgrTest, AddTimer_Nullptr)
{
    auto& mgr = cppbase::TimerMgr::Instance();

    mgr.AddTimer(nullptr);
}

TEST_F(TimerMgrTest, ExceptionHandling)
{
    auto& mgr = cppbase::TimerMgr::Instance();

    auto timer = std::make_shared<cppbase::Timer>(
        100000000, []() {
            throw std::runtime_error("Test exception");
        }
    );

    mgr.AddTimer(timer);

    std::this_thread::sleep_for(std::chrono::milliseconds(150));
}

TEST_F(TimerMgrTest, InvalidParam_Expiration)
{
    EXPECT_THROW(
        (void)std::make_shared<cppbase::Timer>(
            0, [](){}
        ),
        std::invalid_argument
    );

    EXPECT_THROW(
        (void)std::make_shared<cppbase::Timer>(
            -1000000000, [](){}
        ),
        std::invalid_argument
    );
}

TEST_F(TimerMgrTest, InvalidParam_EmptyFunctor)
{
    cppbase::Functor emptyFunc;
    EXPECT_THROW(
        (void)std::make_shared<cppbase::Timer>(
            1000000000, emptyFunc
        ),
        std::invalid_argument
    );
}

TEST_F(TimerMgrTest, TimerGetters)
{
    cppbase::Timer timer(
        1000000000, [](){}, 500000000
    );

    EXPECT_TRUE(timer.Cycle());
    EXPECT_EQ(timer.GetCycleTime(), 500000000);

    auto exp = timer.GetExpiration();
    auto now = std::chrono::steady_clock::now();
    EXPECT_GT(exp, now);
}