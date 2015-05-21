#include "gmock/gmock.h"
#include <thread>
#include <memory>
#include <future>
#include <list>

TEST(Future, ShouldAbleToWaitPromisedValue) {
    std::promise<int> promise;
    auto future = promise.get_future();
    std::thread thread([&promise](){
        // promise.set_value(9527);
        promise.set_value_at_thread_exit(9527);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    });
    ASSERT_TRUE(future.valid());
    //future.wait();
    //future.wait_for(std::chrono::milliseconds(100));
    ASSERT_EQ(9527, future.get());
    thread.join();
}


TEST(Future, ShouldAbleToReceiveExceptionWhenWaitingForPromisedValue) {
    std::promise<int> promise;
    auto future = promise.get_future();
    std::thread([&promise](){
        //promise.set_exception(
        //    std::make_exception_ptr(std::runtime_error("promise error")));
        promise.set_exception_at_thread_exit(
            std::make_exception_ptr(std::runtime_error("promise error")));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }).join();

    ASSERT_TRUE(future.valid());
    try {
        future.get();
        FAIL();
    }
    catch (std::runtime_error &e) {
        ASSERT_STREQ("promise error", e.what());
    }
}

TEST(PackagedTask, ShouldRunTaskAndReturnPromisedValue) {
    std::thread::id taskThreadId;
    std::packaged_task<int(int, int)> task([&taskThreadId](int x, int y){
        taskThreadId = std::this_thread::get_id();
        return x + y;
    });

    auto future = task.get_future();
    
    std::thread([&](){
        task(1, 2);
    }).join();

    ASSERT_EQ(3, future.get());
    ASSERT_NE(taskThreadId, std::this_thread::get_id());
}

TEST(Async, ShouldRunFunctionAsynchronously) {

    bool executed = false;
    auto asyncFuture = std::async(std::launch::async, [&](){
        executed = true;
        return std::this_thread::get_id();
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_TRUE(executed);
    ASSERT_NE(std::this_thread::get_id(), asyncFuture.get());
}

TEST(Async, ShouldRunFunctionOnFutureGet) {

    bool executed = false;
    auto deferredFuture = std::async(std::launch::deferred, [&](){
        executed = true;
        return std::this_thread::get_id();
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_FALSE(executed);
    ASSERT_EQ(std::this_thread::get_id(), deferredFuture.get());
    ASSERT_TRUE(executed);
}

// If both the std::launch::async and std::launch::deferred flags are set in policy, 
// it is up to the implementation whether to perform asynchronous execution or lazy evaluation.

// If neither std::launch::async nor std::launch::deferred, 
// nor any implementation - defined policy flag is set in policy, 
// the behavior is undefined.


TEST(SharedFuture, ShouldAbleToBeWaitByMultipleThreads) {
    std::promise<void> promise;
    std::shared_future<void> future(promise.get_future());

    std::list<std::shared_ptr<std::thread>> threads;
    for (size_t i = 0; i < 10; i++)
    {
        threads.push_back(std::make_shared<std::thread>([&future](){
            future.wait();
        }));
    }

    promise.set_value();

    for (auto thread : threads) {
        thread->join();
    }
}
