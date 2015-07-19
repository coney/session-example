#include "gmock/gmock.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <memory>


TEST(ConditionVariable, ShouldWaitForSpecifiedCondition) {
    bool dataReady = false;
    bool processReady = false;
    std::mutex m;
    std::condition_variable cv;

    std::thread thread([&]() {
        std::unique_lock<std::mutex> l(m);
        printf("waiting for data ready\n");
        cv.wait(l, [&](){return dataReady; });
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        printf("process data\n");
        processReady = true;
        l.unlock();
        printf("process done\n");
        cv.notify_one();
    });

    std::this_thread::sleep_for(std::chrono::microseconds(500));
    {
        std::lock_guard<std::mutex> guard(m);
        dataReady = true;
        cv.notify_one();
    }

    {
        printf("waiting for process ready\n");
        std::unique_lock<std::mutex> l(m);
        cv.wait(l, [&](){return processReady; });
        printf("waiting complete\n");
    }
    thread.join();
}


TEST(ConditionVariable, AllThreadsShouldWaitForSpecifiedCondition) {
    bool dataReady = false;
    std::mutex m;
    std::condition_variable cv;
    std::list<std::shared_ptr<std::thread>> threads;

    for (size_t i = 0; i < 5; i++)
    {
        threads.push_back(std::make_shared<std::thread>([&](size_t i){
            std::unique_lock<std::mutex> l(m);
            cv.wait(l, [&](){ return dataReady; });
            printf("processing in thread %u\n", i);
        }, i));
    }

    std::this_thread::sleep_for(std::chrono::microseconds(500));
    {
        std::lock_guard<std::mutex> guard(m);
        dataReady = true;
        cv.notify_one();
        cv.notify_one();
        cv.notify_one();
        cv.notify_one();
        cv.notify_one();
    }

    for (auto thread : threads) {
        thread->join();
    }

}