#pragma once

#include <mutex>
#include <condition_variable>

class CondVar {
public:
    CondVar();
    
    void wait();
    void notify_one();
    void notify_all();

private:
    std::mutex mutex;
    std::condition_variable cv;
}; 