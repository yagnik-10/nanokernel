#ifndef RWLOCK_HPP
#define RWLOCK_HPP

#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unordered_map>
#include <chrono>

// Lock request types
enum class LockType {
    READ,
    WRITE
};

// Lock request structure
struct LockRequest {
    LockType type;
    uint32_t processId;
    std::chrono::steady_clock::time_point requestTime;
    uint32_t priority;
    bool isGranted;
};

class ReaderWriterLock {
public:
    ReaderWriterLock();
    ~ReaderWriterLock();
    
    // Lock operations
    bool readLock(uint32_t processId, uint32_t priority = 0);
    bool writeLock(uint32_t processId, uint32_t priority = 0);
    void unlock(uint32_t processId);
    
    // Try lock operations with timeout
    bool tryReadLock(uint32_t processId, uint32_t timeout_ms, uint32_t priority = 0);
    bool tryWriteLock(uint32_t processId, uint32_t timeout_ms, uint32_t priority = 0);
    
    // Lock state
    bool isReadLocked() const;
    bool isWriteLocked() const;
    uint32_t getReadLockCount() const;
    uint32_t getWriterProcessId() const;
    
    // Priority inheritance
    void setProcessPriority(uint32_t processId, uint32_t priority);
    uint32_t getProcessPriority(uint32_t processId) const;
    
    // Statistics
    struct LockStats {
        uint32_t readLocks;
        uint32_t writeLocks;
        uint32_t readTimeouts;
        uint32_t writeTimeouts;
        std::chrono::nanoseconds averageReadWaitTime;
        std::chrono::nanoseconds averageWriteWaitTime;
        uint32_t priorityInheritanceCount;
    };
    
    LockStats getStats() const;
    
private:
    // Lock state
    std::mutex mutex_;
    std::condition_variable readCond_;
    std::condition_variable writeCond_;
    
    // Lock tracking
    uint32_t readers_;
    uint32_t writer_;
    bool writerWaiting_;
    
    // Request queues
    std::queue<LockRequest> readQueue_;
    std::queue<LockRequest> writeQueue_;
    
    // Process priorities
    std::unordered_map<uint32_t, uint32_t> processPriorities_;
    
    // Statistics
    LockStats stats_;
    
    // Helper methods
    bool canGrantReadLock() const;
    bool canGrantWriteLock() const;
    void updatePriorityInheritance();
    void processNextRequest();
    
    // Queue management
    void addToQueue(LockRequest& request);
    void removeFromQueue(uint32_t processId);
    void reorderQueues();
    
    // Statistics helpers
    void updateStats(bool isRead, bool isTimeout, std::chrono::nanoseconds waitTime);
    void resetStats();
};

#endif // RWLOCK_HPP 