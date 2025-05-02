#ifndef PCB_HPP
#define PCB_HPP

#include <cstdint>
#include <array>
#include <string>
#include <vector>
#include <chrono>
#include <bitset>

// Process states with more granular control
enum class ProcessState {
    NEW,            // Process is being created
    READY,          // Process is ready to run
    RUNNING,        // Process is currently running
    BLOCKED,        // Process is blocked (waiting for I/O or event)
    SUSPENDED,      // Process is suspended (swapped out)
    TERMINATED      // Process has completed execution
};

// Process priority levels for MLFQ
enum class ProcessPriority {
    REALTIME = 0,   // Highest priority for real-time tasks
    HIGH = 1,       // High priority for interactive tasks
    NORMAL = 2,     // Normal priority for regular tasks
    LOW = 3,        // Low priority for background tasks
    IDLE = 4        // Lowest priority for idle tasks
};

// Process Control Block with advanced features
class PCB {
public:
    // Constructor with priority
    PCB(uint32_t pid, const std::string& name, ProcessPriority priority = ProcessPriority::NORMAL);
    
    // Process identification
    uint32_t getPID() const { return pid_; }
    const std::string& getName() const { return name_; }
    
    // Process state management
    ProcessState getState() const { return state_; }
    void setState(ProcessState state);
    
    // Priority management
    ProcessPriority getPriority() const { return priority_; }
    void setPriority(ProcessPriority priority);
    void boostPriority();  // Priority boosting for aging
    void demotePriority(); // Priority demotion for MLFQ
    
    // CPU state with extended registers
    struct CPUState {
        // General purpose registers
        uint32_t eax, ebx, ecx, edx;
        uint32_t esi, edi, ebp, esp;
        uint32_t eip, eflags;
        
        // Segment registers
        uint32_t cs, ds, es, fs, gs, ss;
        
        // Extended registers
        uint32_t cr0, cr2, cr3, cr4;  // Control registers
        uint32_t dr0, dr1, dr2, dr3;  // Debug registers
        
        // FPU state
        uint8_t fpu_state[512];  // FPU/MMX/SSE state
    };
    
    // Memory management
    struct MemoryInfo {
        uint32_t baseAddress;
        uint32_t limit;
        uint32_t pageTableBase;
        uint32_t stackBase;
        uint32_t heapBase;
        uint32_t heapSize;
    };
    
    const MemoryInfo& getMemoryInfo() const { return memoryInfo_; }
    void setMemoryInfo(const MemoryInfo& info) { memoryInfo_ = info; }
    
    // Resource management
    void addResource(uint32_t resourceId);
    void removeResource(uint32_t resourceId);
    bool hasResource(uint32_t resourceId) const;
    bool requestResource(uint32_t resourceId, uint32_t timeout_ms);
    
    // Process hierarchy
    void setParent(PCB* parent);
    PCB* getParent() const { return parent_; }
    void addChild(PCB* child);
    void removeChild(PCB* child);
    const std::vector<PCB*>& getChildren() const { return children_; }
    
    // Statistics and monitoring
    struct ProcessStats {
        std::chrono::nanoseconds cpuTime;
        std::chrono::nanoseconds userTime;
        std::chrono::nanoseconds systemTime;
        uint32_t pageFaults;
        uint32_t contextSwitches;
        uint32_t ioOperations;
    };
    
    const ProcessStats& getStats() const { return stats_; }
    void updateStats(const ProcessStats& newStats);
    
    // Time quantum management for MLFQ
    uint32_t getTimeQuantum() const { return timeQuantum_; }
    void setTimeQuantum(uint32_t quantum) { timeQuantum_ = quantum; }
    
private:
    // Process identification
    uint32_t pid_;
    std::string name_;
    
    // Process state
    ProcessState state_;
    ProcessPriority priority_;
    
    // CPU state
    CPUState cpuState_;
    
    // Memory management
    MemoryInfo memoryInfo_;
    
    // Resource tracking
    std::bitset<64> resources_;  // Extended to 64 resources
    
    // Process hierarchy
    PCB* parent_;
    std::vector<PCB*> children_;
    
    // Statistics
    ProcessStats stats_;
    
    // Scheduling
    uint32_t timeQuantum_;
    std::chrono::steady_clock::time_point lastRunTime_;
    
    // Helper methods
    void updateStateTransition(ProcessState newState);
    void updatePriorityMetrics();
};

#endif // PCB_HPP 