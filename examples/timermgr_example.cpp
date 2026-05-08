#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <memory>
#include "cppbase/timermgr.hpp"
#include "cppbase/err_code.hpp"

void TestBasicUsage()
{
    std::cout << "=== Test Basic Usage ===" << std::endl;

    auto& mgr = cppbase::TimerMgr::Instance();
    auto timer = std::make_shared<cppbase::Timer>(
        1000000000, [](){
            std::cout << "Timer callback executed!" << std::endl;}
        );

    mgr.AddTimer(timer);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << std::endl;
}

void TestPeriodicTimer()
{
    std::cout << "=== Test Periodic Timer ===" << std::endl;

    auto& mgr = cppbase::TimerMgr::Instance();
    std::atomic<int> counter{0};

    auto timer = std::make_shared<cppbase::Timer>(
        500000000, [&counter](){
            counter++;
            std::cout << "Periodic timer executed, count: " << counter << std::endl;
        }, 500000000
    );

    mgr.AddTimer(timer);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Final counter: " << counter << std::endl;
    std::cout << std::endl;
}

void TestDelTimer()
{
    std::cout << "=== Test DelTimer ===" << std::endl;

    auto& mgr = cppbase::TimerMgr::Instance();
    std::atomic<int> counter{0};

    auto timer = std::make_shared<cppbase::Timer>(
        100000000, [&counter]() {
            counter++;
            std::cout << "Timer executed, count: " << counter << std::endl;
        }, 100000000
    );

    mgr.AddTimer(timer);

    std::this_thread::sleep_for(std::chrono::milliseconds(350));

    int ret = mgr.DelTimer(timer);
    std::cout << "DelTimer result: " << ret << std::endl;

    int oldCounter = counter;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    std::cout << "Counter after delete: " << counter << std::endl;
    std::cout << "Timer stopped successfully: " << (counter == oldCounter ? "yes" : "no") << std::endl;
    std::cout << std::endl;
}

void TestMultipleTimers()
{
    std::cout << "=== Test Multiple Timers ===" << std::endl;

    auto& mgr = cppbase::TimerMgr::Instance();
    std::atomic<int> counter{0};

    for (int i = 1; i <= 5; ++i)
    {
        auto timer = std::make_shared<cppbase::Timer>(
            i * 200000000, [&counter, i]() {
                counter++;
                std::cout << "Timer " << i << " executed!" << std::endl;}
        );
        mgr.AddTimer(timer);
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Total timers executed: " << counter << std::endl;
    std::cout << std::endl;
}

void TestExceptionHandling()
{
    std::cout << "=== Test Exception Handling ===" << std::endl;

    auto& mgr = cppbase::TimerMgr::Instance();

    auto timer = std::make_shared<cppbase::Timer>(
        100000000, []() {
            throw std::runtime_error("Test exception in timer");
        }
    );

    mgr.AddTimer(timer);

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "Timer with exception handled gracefully!" << std::endl;
    std::cout << std::endl;
}

void TestInvalidParam()
{
    std::cout << "=== Test Invalid Parameters ===" << std::endl;

    try
    {
        auto timer = std::make_shared<cppbase::Timer>(
            0, [](){
                std::cout << "Should not reach here" << std::endl;
            }
        );
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << "Caught exception for expiration=0: " << e.what() << std::endl;
    }

    try
    {
        cppbase::Functor emptyFunc;
        auto timer = std::make_shared<cppbase::Timer>(
            1000000000, emptyFunc
        );
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << "Caught exception for empty functor: " << e.what() << std::endl;
    }

    std::cout << std::endl;
}

int main()
{
    TestBasicUsage();
    TestPeriodicTimer();
    TestDelTimer();
    TestMultipleTimers();
    TestExceptionHandling();
    TestInvalidParam();

    std::cout << "All tests completed!" << std::endl;
    return 0;
}