#ifndef IDT_H
#define IDT_H

#include <cstdint>
#include <array>
#include <functional>

// Interrupt descriptor types
enum class InterruptType {
    TASK_GATE = 0x5,
    INTERRUPT_GATE = 0xE,
    TRAP_GATE = 0xF
};

// Interrupt descriptor structure
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
};

// Interrupt handler type
using InterruptHandler = std::function<void()>;

class InterruptDescriptorTable {
public:
    // Singleton instance
    static InterruptDescriptorTable& getInstance();
    
    // Interrupt management
    void setHandler(uint8_t interrupt, InterruptHandler handler);
    void removeHandler(uint8_t interrupt);
    
    // Interrupt descriptor setup
    void setDescriptor(uint8_t interrupt, uint64_t offset, InterruptType type);
    void setDescriptor(uint8_t interrupt, InterruptHandler handler, InterruptType type);
    
    // IDT loading
    void load();
    
    // Interrupt handling
    static void handleInterrupt(uint8_t interrupt);
    
private:
    // Private constructor for singleton
    InterruptDescriptorTable();
    
    // IDT structure
    std::array<InterruptDescriptor, 256> descriptors_;
    std::array<InterruptHandler, 256> handlers_;
    
    // IDTR structure
    struct {
        uint16_t limit;
        uint64_t base;
    } __attribute__((packed)) idtr_;
    
    // Helper methods
    void initialize();
    void setDefaultHandlers();
    static void defaultHandler();
};

#endif // IDT_H 