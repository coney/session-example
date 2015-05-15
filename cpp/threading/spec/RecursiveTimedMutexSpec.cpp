#include "gmock/gmock.h"
#include <list>
#include <thread>
#include <mutex>
#include <memory>

//////////////////////////////////////////////////////////////////////////
// Timed Mutex
TEST(TimedMutex, ShouldReturnFalseOnTimeout) {
    std::timed_mutex guard;
    guard.lock();
    time_t start = time(NULL);
    std::thread([&](){
        ASSERT_FALSE(guard.try_lock_for(std::chrono::seconds(2)));
    }).join();
    ASSERT_GE(time(NULL) - start, 2U);
    guard.unlock();
}

//////////////////////////////////////////////////////////////////////////
// Recursive Mutex
TEST(RecursiveMutex, ShouldAbleToAcquireBySameThreadMultipleTimes) {
    std::mutex guard;
    guard.lock();
    ASSERT_ANY_THROW({
        guard.lock();
    });
    guard.unlock();

    std::recursive_mutex recursiveGuard;
    recursiveGuard.lock();
    recursiveGuard.lock();
    recursiveGuard.lock();
    recursiveGuard.unlock();
    recursiveGuard.unlock();
    recursiveGuard.unlock();
}

//////////////////////////////////////////////////////////////////////////
// Scenario For Recursive Mutex
namespace {
    class Updater {
    public:
        // update continuously
        void updateBatch(int from, int to) {
            std::lock_guard<std::recursive_mutex> guard(updateLock_);
            while (from != to)
            {
                update(from++);
            }
        }

        // update single value
        void update(int value) {
            std::lock_guard<std::recursive_mutex> guard(updateLock_);
            // do something
        }

    private:
        std::recursive_mutex updateLock_;
    };
}
