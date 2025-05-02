#ifndef PAGING_HPP
#define PAGING_HPP

#include <cstdint>
#include <array>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <bitset>

// Page size constants
constexpr uint32_t PAGE_SIZE = 4096;
constexpr uint32_t PAGE_TABLE_ENTRIES = 1024;
constexpr uint32_t PAGE_DIRECTORY_ENTRIES = 1024;

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

// Page replacement algorithms
enum class PageReplacementAlgorithm {
    CLOCK,
    LRU,
    FIFO,
    SECOND_CHANCE
};

// Page table entry structure with extended features
struct PageTableEntry {
    uint64_t value;
    
    // Getters
    bool isPresent() const { return value & PRESENT; }
    bool isWritable() const { return value & WRITABLE; }
    bool isUserMode() const { return value & USER_MODE; }
    bool isAccessed() const { return value & ACCESSED; }
    bool isDirty() const { return value & DIRTY; }
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
    void setAccessed(bool accessed) {
        value = (value & ~ACCESSED) | (accessed ? ACCESSED : 0);
    }
    void setDirty(bool dirty) {
        value = (value & ~DIRTY) | (dirty ? DIRTY : 0);
    }
    void setPhysicalAddress(uint64_t address) {
        value = (value & 0xFFF0000000000FFF) | (address & 0x000FFFFFFFFFF000);
    }
};

// Page table structure with advanced features
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
    
    // Page replacement
    void setPageReplacementAlgorithm(PageReplacementAlgorithm algorithm);
    uint32_t findVictimPage();
    
    // Memory protection
    bool protectPage(uint32_t virtualAddr, uint32_t flags);
    bool isPageAccessible(uint32_t virtualAddr, bool write) const;
    
    // Statistics
    struct PageTableStats {
        uint32_t totalPages;
        uint32_t usedPages;
        uint32_t pageFaults;
        uint32_t pageReplacements;
        uint32_t dirtyPages;
    };
    
    PageTableStats getStats() const;
    
private:
    // Page table levels
    std::array<PageTableEntry, PAGE_DIRECTORY_ENTRIES> pageDirectory_;
    std::vector<std::array<PageTableEntry, PAGE_TABLE_ENTRIES>> pageTables_;
    
    // Memory management
    std::vector<uint32_t> freePages_;
    uint32_t nextFreePage_;
    
    // Page replacement
    PageReplacementAlgorithm replacementAlgorithm_;
    std::vector<uint32_t> pageFrames_;
    std::unordered_map<uint32_t, size_t> pageAccessCount_;
    std::bitset<PAGE_TABLE_ENTRIES> referenceBits_;
    size_t clockHand_;
    
    // Statistics
    PageTableStats stats_;
    
    // Synchronization
    mutable std::mutex pageTableMutex_;
    
    // Helper methods
    bool allocatePageTable(uint32_t directoryIndex);
    void freePageTable(uint32_t directoryIndex);
    uint32_t getPageTableIndex(uint32_t virtualAddr) const;
    uint32_t getPageIndex(uint32_t virtualAddr) const;
    
    // Page replacement helpers
    uint32_t findVictimPageClock();
    uint32_t findVictimPageLRU();
    uint32_t findVictimPageFIFO();
    uint32_t findVictimPageSecondChance();
    
    // Memory protection helpers
    bool checkPagePermissions(uint32_t virtualAddr, bool write) const;
    void updatePageAccess(uint32_t virtualAddr);
};

#endif // PAGING_HPP 