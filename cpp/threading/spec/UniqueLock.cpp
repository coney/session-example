#include "gmock/gmock.h"
#include <list>
#include <thread>
#include <mutex>
#include <memory>

TEST(UniqueLock, ShouldAbleToWrapALockedMutex) {
    std::mutex m;
    std::unique_lock<std::mutex> l(m);
    ASSERT_TRUE(l.owns_lock());
    l.unlock();
    ASSERT_FALSE(l.owns_lock());
}

TEST(UniqueLock, ShouldAbleToWrapMutexWithoutLockIt) {
    std::mutex m;
    std::unique_lock<std::mutex> l(m, std::defer_lock);
    ASSERT_FALSE(l.owns_lock());
}

TEST(UniqueLock, ShouldAbleToWrapMutexWithTryLock) {
    std::mutex m;
    m.lock();
    std::unique_lock<std::mutex> l(m, std::adopt_lock);
    ASSERT_TRUE(l.owns_lock());
    l.unlock();
    ASSERT_FALSE(l.owns_lock());
}

TEST(UniqueLock, ShouldAbleToAvoidDeadLock) {
    {
        std::mutex m1, m2;
        std::thread thread([&](){
            m1.lock();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            m2.lock();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            m2.unlock();
            m1.unlock();
        });

        m1.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        m2.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        m2.unlock();
        m1.unlock();

        thread.join();
    };

    //{
    //    std::mutex m1, m2;
    //    std::thread thread([&](){
    //        m2.lock();
    //        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //        m1.lock();
    //        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //        m1.unlock();
    //        m2.unlock();
    //    });

    //    m1.lock();
    //    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //    m2.lock();
    //    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //    m2.unlock();
    //    m1.unlock();

    //    thread.join();
    //};

    {
        std::mutex m1, m2;
        std::thread thread([&](){
            std::lock(m1, m2);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            m2.unlock();
            m1.unlock();
        });

        std::lock(m2, m1);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        m2.unlock();
        m1.unlock();

        thread.join();
    };

}