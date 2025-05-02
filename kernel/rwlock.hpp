#pragma once

#include <mutex>
#include <condition_variable>

class RWLock {
public:
    RWLock();
    
    void read_lock();
    void read_unlock();
    void write_lock();
    void write_unlock();

private:
    std::mutex mutex;
    std::condition_variable cv;
    int readers;
    bool writer;
}; 