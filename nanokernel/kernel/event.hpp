#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>

struct Event {
    int type;
    int data;
};

class EventSystem {
public:
    EventSystem();
    
    void post(const Event& event);
    bool wait_for(const Event& event, int timeout_ms = 0);
    size_t get_event_count();

private:
    std::vector<Event> events;
    mutable std::mutex mutex;
    std::condition_variable cv;
    size_t event_count;
}; 