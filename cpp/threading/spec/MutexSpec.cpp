#include "gmock/gmock.h"
#include <list>
#include <thread>
#include <mutex>
#include <memory>

TEST(Mutex, ShouldNotSerialiseOperationInScopeWithoutMutex) {
    std::list<std::shared_ptr<std::thread>> threads;

    unsigned int count = 0;
    for (size_t i = 0; i < 10; ++i) {
        threads.push_back(std::make_shared<std::thread>([](unsigned int &count){
            for (size_t j = 0; j < 10; ++j)
            {
                unsigned int intVal = count;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                ++intVal;
                count = intVal;
            }
        }, std::ref(count)));
    }

    std::for_each(threads.begin(), threads.end(), [](std::shared_ptr<std::thread> &thread) {
        thread->join();
    });

    printf("%u\n", count);
    ASSERT_LT(count, 100U);
}

TEST(Mutex, ShouldSerialiseOperationInScopeWithMutex) {
    std::list<std::shared_ptr<std::thread>> threads;
    std::mutex countGuard;
    unsigned int count = 0;
    for (size_t i = 0; i < 10; ++i) {
        threads.push_back(std::make_shared<std::thread>([&](unsigned int &count){
            for (size_t j = 0; j < 10; ++j)
            {
                countGuard.lock();
                unsigned int intVal = count;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                ++intVal;
                count = intVal;
                countGuard.unlock();
            }
        }, std::ref(count)));
    }

    std::for_each(threads.begin(), threads.end(), [](std::shared_ptr<std::thread> &thread) {
        thread->join();
    });

    ASSERT_EQ(100U, count);
}

TEST(Mutex, ShouldAutomaticlyLockAndUnlockMutexWithLockGuard) {
    std::list<std::shared_ptr<std::thread>> threads;
    std::mutex countGuard;
    unsigned int count = 0;
    for (size_t i = 0; i < 10; ++i) {
        threads.push_back(std::make_shared<std::thread>([&](unsigned int &count){
            for (size_t j = 0; j < 10; ++j)
            {
                std::lock_guard<std::mutex> scopeLock(countGuard);
                unsigned int intVal = count;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                ++intVal;
                count = intVal;
            }
        }, std::ref(count)));
    }

    std::for_each(threads.begin(), threads.end(), [](std::shared_ptr<std::thread> &thread) {
        thread->join();
    });

    ASSERT_EQ(100U, count);
}

TEST(Mutex, TryLockShouldReturnImmediately) {
    std::list<std::shared_ptr<std::thread>> threads;
    std::mutex countGuard;
    unsigned int count = 0;
    for (size_t i = 0; i < 10; ++i) {
        threads.push_back(std::make_shared<std::thread>([&](unsigned int &count){
            for (size_t j = 0; j < 10; ++j)
            {
                while (!countGuard.try_lock()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                unsigned int intVal = count;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                ++intVal;
                count = intVal;
                countGuard.unlock();
            }
        }, std::ref(count)));
    }

    std::for_each(threads.begin(), threads.end(), [](std::shared_ptr<std::thread> &thread) {
        thread->join();
    });

    ASSERT_EQ(100U, count);
}