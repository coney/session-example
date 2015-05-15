
#include "gmock/gmock.h"
#include <thread>
#include <memory>

//////////////////////////////////////////////////////////////////////////
// Create Thread From Static Function And Join Thread
void thread_proc(int &arg) {
    arg = 9527;
}

TEST(Thread, ShouldAbleToCreateNewThreadByStaticFunction) {
    int val = 0;
    std::thread thread(thread_proc, std::ref(val));
    thread.join();
    ASSERT_EQ(9527, val);
}

//////////////////////////////////////////////////////////////////////////
// Create Thread From Lambda
TEST(Thread, ShouldAbleToCreateNewThreadByLambda) {
    bool flag = false;
    std::thread thread([](bool & innerFlag) {
        innerFlag = true;
    }, std::ref(flag));
    thread.join();
    ASSERT_TRUE(flag);
}



//////////////////////////////////////////////////////////////////////////
// Create Thread From Member Function
class ThreadRunner {
public:
    ThreadRunner()
        : result_(0) {

    }

    void run(int val) {
        result_ = val;
    }

    int getResult() const { return result_; }

private:
    int result_;
};

TEST(Thread, ShouldAbleToCreateNewThreadByMemberFunction) {
    ThreadRunner runner;
    std::thread thread(&ThreadRunner::run, &runner, 896);
    thread.join();
    ASSERT_EQ(896, runner.getResult());
}

//////////////////////////////////////////////////////////////////////////
// Identify Threads By get_id()
TEST(Thread, ShouldAbleToGetCurrentId) {
    std::thread::id newThreadId;
    std::thread thread([&](){
        newThreadId = std::this_thread::get_id();
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("thread id: %08X\n", thread.get_id().hash());
    ASSERT_EQ(newThreadId, thread.get_id());
    thread.join();
}

//////////////////////////////////////////////////////////////////////////
// Detach Running Thread From Instance
TEST(Thread, ShouldAbleToDetach) {
    ASSERT_DEATH({
        std::auto_ptr<std::thread> thread(new std::thread([](){
            for (size_t i = 0; i < 5; i++)
            {
                printf(".");
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }));
        // terminated in ~thread()
        thread.reset();
    }, "");

    std::auto_ptr<std::thread> thread(new std::thread([](){
        for (size_t i = 0; i < 5; i++)
        {
            printf(".");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }));

    // this thread variable now has nothing to do with the running thread
    thread->detach();
    thread.reset();
}

