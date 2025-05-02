#include "condvar.hpp"

CondVar::CondVar() {}

void CondVar::wait() {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock);
}

void CondVar::notify_one() {
    cv.notify_one();
}

void CondVar::notify_all() {
    cv.notify_all();
} 