#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>

struct Event {
    std::string name;
    int data;
};

class EventSystem {
public:
    EventSystem();
    
    void post(const std::string& event_name, int data);
    bool wait_for(const std::string& event_name, int& data, int timeout_ms = 0);
    size_t get_event_count() const;

private:
    std::vector<Event> events;
    std::mutex mutex;
    std::condition_variable cv;
    size_t event_count;
}; 