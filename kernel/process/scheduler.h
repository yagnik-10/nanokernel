#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "pcb.h"
#include <queue>
#include <unordered_map>
#include <mutex>

class Scheduler {
public:
    // Singleton instance
    static Scheduler& getInstance();
    
    // Process management
    void addProcess(PCB* process);
    void removeProcess(PCB* process);
    PCB* getNextProcess();
    
    // Scheduling policies
    enum class Policy {
        ROUND_ROBIN,
        PRIORITY,
        MULTILEVEL_FEEDBACK
    };
    
    void setPolicy(Policy policy) { policy_ = policy; }
    Policy getPolicy() const { return policy_; }
    
    // Time quantum management
    void setTimeQuantum(uint32_t quantum) { timeQuantum_ = quantum; }
    uint32_t getTimeQuantum() const { return timeQuantum_; }
    
    // Process state transitions
    void blockProcess(PCB* process);
    void unblockProcess(PCB* process);
    void terminateProcess(PCB* process);
    
    // Statistics
    struct SchedulerStats {
        uint32_t totalProcesses;
        uint32_t runningProcesses;
        uint32_t blockedProcesses;
        uint32_t contextSwitches;
        uint32_t totalRuntime;
    };
    
    SchedulerStats getStats() const;
    
private:
    // Private constructor for singleton
    Scheduler();
    
    // Scheduling queues
    std::queue<PCB*> readyQueue_;
    std::unordered_map<uint32_t, PCB*> blockedProcesses_;
    std::unordered_map<uint32_t, PCB*> allProcesses_;
    
    // Scheduling policy
    Policy policy_;
    uint32_t timeQuantum_;
    
    // Statistics
    SchedulerStats stats_;
    
    // Synchronization
    mutable std::mutex schedulerMutex_;
    
    // Helper methods
    void updateStats();
    void handleProcessTermination(PCB* process);
    void handleProcessBlocking(PCB* process);
    void handleProcessUnblocking(PCB* process);
};

#endif // SCHEDULER_H 