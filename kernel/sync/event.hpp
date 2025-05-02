#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdint>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <functional>
#include <chrono>
#include <vector>

// Event types
enum class EventType {
    SYSTEM,
    USER,
    INTERRUPT,
    TIMER,
    IO,
    CUSTOM
};

// Event priority levels
enum class EventPriority {
    CRITICAL = 0,
    HIGH = 1,
    NORMAL = 2,
    LOW = 3
};

// Event structure
struct Event {
    uint32_t id;
    EventType type;
    EventPriority priority;
    std::chrono::steady_clock::time_point timestamp;
    std::vector<uint8_t> data;
    bool isHandled;
};

// Event handler type
using EventHandler = std::function<void(const Event&)>;

// Event subscription structure
struct EventSubscription {
    EventType type;
    EventHandler handler;
    EventPriority priority;
    bool isActive;
    std::chrono::steady_clock::time_point lastHandled;
    uint32_t handledCount;
};

class EventSystem {
public:
    // Singleton instance
    static EventSystem& getInstance();
    
    // Event management
    void postEvent(const Event& event);
    void postEventWithPriority(const Event& event, EventPriority priority);
    void postEventWithDelay(const Event& event, std::chrono::nanoseconds delay);
    
    // Event subscription
    uint32_t subscribe(EventType type, EventHandler handler, EventPriority priority = EventPriority::NORMAL);
    void unsubscribe(uint32_t subscriptionId);
    void pauseSubscription(uint32_t subscriptionId);
    void resumeSubscription(uint32_t subscriptionId);
    
    // Event processing
    void processEvents();
    void processEvent(const Event& event);
    void processPendingEvents();
    
    // Event queue management
    void clearEventQueue();
    void removeEvent(uint32_t eventId);
    void reorderEventQueue();
    
    // Statistics
    struct EventStats {
        uint32_t totalEvents;
        uint32_t handledEvents;
        uint32_t pendingEvents;
        std::array<uint32_t, 4> priorityDistribution;
        std::chrono::nanoseconds averageProcessingTime;
        std::chrono::nanoseconds averageQueueTime;
        uint32_t subscriptionCount;
    };
    
    EventStats getStats() const;
    
private:
    // Private constructor for singleton
    EventSystem();
    
    // Event queues
    std::array<std::queue<Event>, 4> eventQueues_;  // One queue per priority
    std::unordered_map<uint32_t, Event> pendingEvents_;
    
    // Event subscriptions
    std::unordered_map<uint32_t, EventSubscription> subscriptions_;
    std::unordered_map<EventType, std::vector<uint32_t>> typeSubscriptions_;
    
    // Statistics
    EventStats stats_;
    
    // Synchronization
    mutable std::mutex eventMutex_;
    
    // Helper methods
    void processEventQueue(std::queue<Event>& queue);
    void updateEventPriority(Event& event);
    void handleEvent(const Event& event, const EventSubscription& subscription);
    
    // Queue management
    void addToQueue(const Event& event);
    void removeFromQueue(uint32_t eventId);
    void reorderQueues();
    
    // Statistics helpers
    void updateStats(bool isHandled, std::chrono::nanoseconds processingTime);
    void updateSubscriptionStats(const EventSubscription& subscription);
    void resetStats();
    
    // Time tracking
    std::chrono::steady_clock::time_point lastProcessTime_;
};

#endif // EVENT_HPP 