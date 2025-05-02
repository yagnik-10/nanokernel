#ifndef CONDVAR_HPP
#define CONDVAR_HPP

#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <functional>

// Condition variable predicate type
using Predicate = std::function<bool()>;

// Wait request structure
struct WaitRequest {
    uint32_t processId;
    Predicate predicate;
    std::chrono::steady_clock::time_point requestTime;
    std::chrono::nanoseconds timeout;
    bool isTimeout;
};

class ConditionVariable {
public:
    ConditionVariable();
    ~ConditionVariable();
    
    // Wait operations
    void wait(std::unique_lock<std::mutex>& lock, Predicate predicate);
    bool waitFor(std::unique_lock<std::mutex>& lock, 
                std::chrono::nanoseconds timeout,
                Predicate predicate);
    bool waitUntil(std::unique_lock<std::mutex>& lock,
                  std::chrono::steady_clock::time_point timeout,
                  Predicate predicate);
    
    // Notification operations
    void notifyOne();
    void notifyAll();
    
    // Wait queue management
    void removeFromQueue(uint32_t processId);
    void clearQueue();
    
    // Statistics
    struct CondVarStats {
        uint32_t totalWaits;
        uint32_t successfulWaits;
        uint32_t timeoutWaits;
        uint32_t notifications;
        std::chrono::nanoseconds averageWaitTime;
        std::chrono::nanoseconds averageTimeoutTime;
        uint32_t queueSize;
    };
    
    CondVarStats getStats() const;
    
private:
    // Condition variable
    std::condition_variable cv_;
    
    // Wait queue
    std::queue<WaitRequest> waitQueue_;
    
    // Statistics
    CondVarStats stats_;
    
    // Helper methods
    bool checkPredicate(const Predicate& predicate);
    void processWaitQueue();
    void updateStats(bool isTimeout, std::chrono::nanoseconds waitTime);
    
    // Queue management
    void addToQueue(const WaitRequest& request);
    void removeFromQueue();
    void reorderQueue();
    
    // Timeout handling
    bool handleTimeout(WaitRequest& request);
    void checkTimeouts();
    
    // Statistics helpers
    void updateWaitStats(bool isTimeout, std::chrono::nanoseconds waitTime);
    void updateNotificationStats();
    void resetStats();
};

#endif // CONDVAR_HPP 