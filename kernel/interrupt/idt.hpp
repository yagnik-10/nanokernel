#ifndef IDT_HPP
#define IDT_HPP

#include <cstdint>
#include <array>
#include <functional>
#include <vector>
#include <mutex>
#include <chrono>

// Interrupt descriptor types
enum class InterruptType {
    TASK_GATE = 0x5,
    INTERRUPT_GATE = 0xE,
    TRAP_GATE = 0xF
};

// Interrupt priority levels
enum class InterruptPriority {
    CRITICAL = 0,    // Highest priority (e.g., NMI)
    HIGH = 1,        // High priority (e.g., timer)
    NORMAL = 2,      // Normal priority (e.g., device)
    LOW = 3          // Low priority (e.g., software)
};

// Interrupt descriptor structure with extended features
struct InterruptDescriptor {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
    
    void setOffset(uint64_t offset) {
        offset_low = offset & 0xFFFF;
        offset_mid = (offset >> 16) & 0xFFFF;
        offset_high = (offset >> 32) & 0xFFFFFFFF;
    }
    
    void setType(InterruptType type) {
        type_attr = (type_attr & 0xF0) | static_cast<uint8_t>(type);
    }
    
    void setPresent(bool present) {
        type_attr = (type_attr & 0x7F) | (present ? 0x80 : 0);
    }
    
    void setPriority(InterruptPriority priority) {
        type_attr = (type_attr & 0xCF) | (static_cast<uint8_t>(priority) << 4);
    }
};

// Interrupt handler type with context
struct InterruptContext {
    uint32_t eax, ebx, ecx, edx;
    uint32_t esi, edi, ebp, esp;
    uint32_t eip, eflags;
    uint32_t cs, ds, es, fs, gs, ss;
    uint32_t error_code;
    uint32_t interrupt_number;
};

using InterruptHandler = std::function<void(InterruptContext&)>;

// Interrupt handler chain
struct InterruptHandlerChain {
    InterruptHandler handler;
    InterruptPriority priority;
    bool enabled;
    std::chrono::nanoseconds lastExecutionTime;
    uint32_t executionCount;
};

class InterruptDescriptorTable {
public:
    // Singleton instance
    static InterruptDescriptorTable& getInstance();
    
    // Interrupt management
    void setHandler(uint8_t interrupt, InterruptHandler handler, InterruptPriority priority = InterruptPriority::NORMAL);
    void removeHandler(uint8_t interrupt);
    void enableInterrupt(uint8_t interrupt);
    void disableInterrupt(uint8_t interrupt);
    
    // Interrupt descriptor setup
    void setDescriptor(uint8_t interrupt, uint64_t offset, InterruptType type, InterruptPriority priority);
    void setDescriptor(uint8_t interrupt, InterruptHandler handler, InterruptType type, InterruptPriority priority);
    
    // IDT loading
    void load();
    
    // Interrupt handling
    static void handleInterrupt(uint8_t interrupt, InterruptContext& context);
    
    // Statistics
    struct InterruptStats {
        uint32_t totalInterrupts;
        std::array<uint32_t, 256> interruptCounts;
        std::chrono::nanoseconds totalProcessingTime;
        std::array<std::chrono::nanoseconds, 256> averageProcessingTimes;
    };
    
    InterruptStats getStats() const;
    
private:
    // Private constructor for singleton
    InterruptDescriptorTable();
    
    // IDT structure
    std::array<InterruptDescriptor, 256> descriptors_;
    std::array<std::vector<InterruptHandlerChain>, 256> handlerChains_;
    
    // IDTR structure
    struct {
        uint16_t limit;
        uint64_t base;
    } __attribute__((packed)) idtr_;
    
    // Statistics
    InterruptStats stats_;
    
    // Synchronization
    mutable std::mutex idtMutex_;
    
    // Helper methods
    void initialize();
    void setDefaultHandlers();
    static void defaultHandler(InterruptContext& context);
    
    // Statistics helpers
    void updateStats(uint8_t interrupt, std::chrono::nanoseconds processingTime);
    void resetStats();
    
    // Handler chain management
    void sortHandlerChain(uint8_t interrupt);
    bool executeHandlerChain(uint8_t interrupt, InterruptContext& context);
};

#endif // IDT_HPP 