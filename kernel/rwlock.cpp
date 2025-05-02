#include "rwlock.hpp"
#include <stdexcept>

RWLock::RWLock() : readers(0), writer(false) {}

void RWLock::read_lock() {
    std::unique_lock<std::mutex> lock(mutex);
    while (writer) {
        cv.wait(lock);
    }
    readers++;
}

void RWLock::read_unlock() {
    std::unique_lock<std::mutex> lock(mutex);
    if (readers > 0) {
        readers--;
        if (readers == 0) {
            cv.notify_all();
        }
    }
}

void RWLock::write_lock() {
    std::unique_lock<std::mutex> lock(mutex);
    while (writer || readers > 0) {
        cv.wait(lock);
    }
    writer = true;
}

void RWLock::write_unlock() {
    std::unique_lock<std::mutex> lock(mutex);
    writer = false;
    cv.notify_all();
} 