#ifndef PAGING_H
#define PAGING_H

#include <cstdint>
#include <array>
#include <vector>
#include <mutex>

// Page size constants
constexpr uint32_t PAGE_SIZE = 4096;
constexpr uint32_t PAGE_TABLE_ENTRIES = 1024;

// Page table entry flags
enum PageFlags {
    PRESENT = 1 << 0,
    WRITABLE = 1 << 1,
    USER_MODE = 1 << 2,
    WRITE_THROUGH = 1 << 3,
    CACHE_DISABLED = 1 << 4,
    ACCESSED = 1 << 5,
    DIRTY = 1 << 6,
    HUGE_PAGE = 1 << 7,
    GLOBAL = 1 << 8,
    NO_EXECUTE = 1 << 63
};

// Page table entry structure
struct PageTableEntry {
    uint64_t value;
    
    // Getters
    bool isPresent() const { return value & PRESENT; }
    bool isWritable() const { return value & WRITABLE; }
    bool isUserMode() const { return value & USER_MODE; }
    uint64_t getPhysicalAddress() const { return value & 0x000FFFFFFFFFF000; }
    
    // Setters
    void setPresent(bool present) {
        value = (value & ~PRESENT) | (present ? PRESENT : 0);
    }
    void setWritable(bool writable) {
        value = (value & ~WRITABLE) | (writable ? WRITABLE : 0);
    }
    void setUserMode(bool userMode) {
        value = (value & ~USER_MODE) | (userMode ? USER_MODE : 0);
    }
    void setPhysicalAddress(uint64_t address) {
        value = (value & 0xFFF0000000000FFF) | (address & 0x000FFFFFFFFFF000);
    }
};

// Page table structure
class PageTable {
public:
    PageTable();
    ~PageTable();
    
    // Page table operations
    bool mapPage(uint32_t virtualAddr, uint32_t physicalAddr, uint32_t flags);
    bool unmapPage(uint32_t virtualAddr);
    uint32_t getPhysicalAddress(uint32_t virtualAddr) const;
    
    // Page table traversal
    PageTableEntry* getPageTableEntry(uint32_t virtualAddr);
    const PageTableEntry* getPageTableEntry(uint32_t virtualAddr) const;
    
    // Memory allocation
    uint32_t allocatePage();
    void freePage(uint32_t physicalAddr);
    
    // Page table switching
    void activate();
    static void invalidatePage(uint32_t virtualAddr);
    
private:
    // Page table levels
    std::array<PageTableEntry, PAGE_TABLE_ENTRIES> pageDirectory_;
    std::vector<std::array<PageTableEntry, PAGE_TABLE_ENTRIES>> pageTables_;
    
    // Memory management
    std::vector<uint32_t> freePages_;
    uint32_t nextFreePage_;
    
    // Synchronization
    mutable std::mutex pageTableMutex_;
    
    // Helper methods
    bool allocatePageTable(uint32_t directoryIndex);
    void freePageTable(uint32_t directoryIndex);
    uint32_t getPageTableIndex(uint32_t virtualAddr) const;
    uint32_t getPageIndex(uint32_t virtualAddr) const;
};

#endif // PAGING_H 