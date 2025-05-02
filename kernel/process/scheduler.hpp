#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "pcb.hpp"
#include <queue>
#include <unordered_map>
#include <mutex>
#include <chrono>
#include <array>

class Scheduler {
public:
    // Singleton instance
    static Scheduler& getInstance();
    
    // Process management
    void addProcess(PCB* process);
    void removeProcess(PCB* process);
    PCB* getNextProcess();
    
    // MLFQ specific methods
    void boostPriorities();  // Priority boosting for aging
    void updateTimeQuantum(PCB* process);
    
    // Process state transitions
    void blockProcess(PCB* process);
    void unblockProcess(PCB* process);
    void terminateProcess(PCB* process);
    void suspendProcess(PCB* process);
    void resumeProcess(PCB* process);
    
    // Scheduling parameters
    struct SchedulerConfig {
        std::array<uint32_t, 5> timeQuanta;  // Time quanta for each priority level
        uint32_t boostInterval;              // Priority boosting interval in ms
        uint32_t maxPriority;                // Maximum priority level
        bool preemptive;                     // Whether scheduling is preemptive
    };
    
    void setConfig(const SchedulerConfig& config);
    const SchedulerConfig& getConfig() const { return config_; }
    
    // Statistics
    struct SchedulerStats {
        uint32_t totalProcesses;
        uint32_t runningProcesses;
        uint32_t blockedProcesses;
        uint32_t suspendedProcesses;
        uint32_t contextSwitches;
        std::chrono::nanoseconds totalRuntime;
        std::array<uint32_t, 5> priorityDistribution;
        std::chrono::nanoseconds averageResponseTime;
        std::chrono::nanoseconds averageTurnaroundTime;
    };
    
    SchedulerStats getStats() const;
    
private:
    // Private constructor for singleton
    Scheduler();
    
    // MLFQ queues for each priority level
    std::array<std::queue<PCB*>, 5> readyQueues_;
    std::unordered_map<uint32_t, PCB*> blockedProcesses_;
    std::unordered_map<uint32_t, PCB*> suspendedProcesses_;
    std::unordered_map<uint32_t, PCB*> allProcesses_;
    
    // Scheduling configuration
    SchedulerConfig config_;
    
    // Statistics
    SchedulerStats stats_;
    
    // Synchronization
    mutable std::mutex schedulerMutex_;
    
    // Helper methods
    void updateStats();
    void handleProcessTermination(PCB* process);
    void handleProcessBlocking(PCB* process);
    void handleProcessUnblocking(PCB* process);
    void handleProcessSuspension(PCB* process);
    void handleProcessResumption(PCB* process);
    
    // MLFQ specific helpers
    void demoteProcess(PCB* process);
    void promoteProcess(PCB* process);
    bool shouldBoostPriorities() const;
    void updateProcessMetrics(PCB* process);
    
    // Time tracking
    std::chrono::steady_clock::time_point lastBoostTime_;
    std::chrono::steady_clock::time_point lastStatsUpdate_;
};

#endif // SCHEDULER_HPP 