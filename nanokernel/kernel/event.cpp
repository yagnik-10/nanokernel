#include "event.hpp"
#include <chrono>

EventSystem::EventSystem() : event_count(0) {}

void EventSystem::post(const Event& event) {
    std::unique_lock<std::mutex> lock(mutex);
    events.push_back(event);
    event_count++;
    cv.notify_all();
}

bool EventSystem::wait_for(const Event& event, int timeout_ms) {
    std::unique_lock<std::mutex> lock(mutex);
    auto start = std::chrono::steady_clock::now();
    auto timeout = std::chrono::milliseconds(timeout_ms);

    while (true) {
        for (const auto& e : events) {
            if (e.type == event.type && e.data == event.data) {
                return true;
            }
        }

        if (timeout_ms > 0) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
            if (elapsed >= timeout) {
                return false;
            }
            cv.wait_for(lock, timeout - elapsed);
        } else {
            cv.wait(lock);
        }
    }
}

size_t EventSystem::get_event_count() {
    std::unique_lock<std::mutex> lock(mutex);
    return event_count;
} 