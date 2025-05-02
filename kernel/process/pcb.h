#ifndef PCB_H
#define PCB_H

#include <cstdint>
#include <array>
#include <string>

// Process states
enum class ProcessState {
    NEW,        // Process is being created
    READY,      // Process is ready to run
    RUNNING,    // Process is currently running
    BLOCKED,    // Process is blocked (waiting for I/O or event)
    TERMINATED  // Process has completed execution
};

// Process Control Block
class PCB {
public:
    // Constructor
    PCB(uint32_t pid, const std::string& name);
    
    // Process identification
    uint32_t getPID() const { return pid_; }
    const std::string& getName() const { return name_; }
    
    // Process state management
    ProcessState getState() const { return state_; }
    void setState(ProcessState state) { state_ = state; }
    
    // CPU state
    struct CPUState {
        uint32_t eax, ebx, ecx, edx;
        uint32_t esi, edi, ebp, esp;
        uint32_t eip, eflags;
        uint32_t cs, ds, es, fs, gs, ss;
    };
    
    // Memory management
    uint32_t getBaseAddress() const { return baseAddress_; }
    uint32_t getLimit() const { return limit_; }
    void setMemoryRange(uint32_t base, uint32_t limit) {
        baseAddress_ = base;
        limit_ = limit;
    }
    
    // Scheduling
    uint32_t getPriority() const { return priority_; }
    void setPriority(uint32_t priority) { priority_ = priority; }
    uint32_t getTimeSlice() const { return timeSlice_; }
    void setTimeSlice(uint32_t time) { timeSlice_ = time; }
    
    // Resource management
    void addResource(uint32_t resourceId);
    void removeResource(uint32_t resourceId);
    bool hasResource(uint32_t resourceId) const;
    
    // Parent-child relationship
    void setParent(PCB* parent) { parent_ = parent; }
    PCB* getParent() const { return parent_; }
    void addChild(PCB* child);
    void removeChild(PCB* child);
    
private:
    // Process identification
    uint32_t pid_;
    std::string name_;
    
    // Process state
    ProcessState state_;
    
    // CPU state
    CPUState cpuState_;
    
    // Memory management
    uint32_t baseAddress_;
    uint32_t limit_;
    
    // Scheduling
    uint32_t priority_;
    uint32_t timeSlice_;
    
    // Resource tracking
    std::array<bool, 32> resources_;
    
    // Process hierarchy
    PCB* parent_;
    std::vector<PCB*> children_;
};

#endif // PCB_H 