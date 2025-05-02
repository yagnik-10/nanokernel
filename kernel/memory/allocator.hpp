#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <cstdint>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <memory>

// Memory allocation strategies
enum class AllocationStrategy {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT,
    BUDDY_SYSTEM
};

// Memory block structure
struct MemoryBlock {
    uint32_t address;
    uint32_t size;
    bool isFree;
    MemoryBlock* next;
    MemoryBlock* prev;
    
    // Memory block metadata
    uint32_t allocationCount;
    std::chrono::steady_clock::time_point lastAccess;
    uint32_t fragmentationScore;
};

class MemoryAllocator {
public:
    // Singleton instance
    static MemoryAllocator& getInstance();
    
    // Memory allocation
    void* allocate(uint32_t size, uint32_t alignment = 4);
    void deallocate(void* ptr);
    
    // Memory management
    void setAllocationStrategy(AllocationStrategy strategy);
    void defragment();
    void compact();
    
    // Memory protection
    bool protectRegion(void* ptr, uint32_t size, uint32_t flags);
    bool isRegionAccessible(void* ptr, uint32_t size, bool write) const;
    
    // Memory statistics
    struct AllocatorStats {
        uint32_t totalMemory;
        uint32_t usedMemory;
        uint32_t freeMemory;
        uint32_t allocationCount;
        uint32_t deallocationCount;
        uint32_t fragmentationCount;
        std::chrono::nanoseconds averageAllocationTime;
        std::chrono::nanoseconds averageDeallocationTime;
    };
    
    AllocatorStats getStats() const;
    
private:
    // Private constructor for singleton
    MemoryAllocator();
    
    // Memory block management
    std::vector<MemoryBlock*> freeBlocks_;
    std::unordered_map<void*, MemoryBlock*> allocatedBlocks_;
    
    // Allocation strategy
    AllocationStrategy strategy_;
    
    // Statistics
    AllocatorStats stats_;
    
    // Synchronization
    mutable std::mutex allocatorMutex_;
    
    // Helper methods
    MemoryBlock* findFreeBlock(uint32_t size);
    MemoryBlock* findBestFit(uint32_t size);
    MemoryBlock* findWorstFit(uint32_t size);
    MemoryBlock* findFirstFit(uint32_t size);
    MemoryBlock* allocateBuddyBlock(uint32_t size);
    
    // Memory block operations
    void splitBlock(MemoryBlock* block, uint32_t size);
    void mergeBlocks(MemoryBlock* block);
    void updateBlockMetadata(MemoryBlock* block);
    
    // Statistics helpers
    void updateStats(bool isAllocation, std::chrono::nanoseconds time);
    void updateFragmentationStats();
    
    // Memory protection helpers
    bool checkRegionPermissions(void* ptr, uint32_t size, bool write) const;
    void updateRegionAccess(void* ptr, uint32_t size);
};

#endif // ALLOCATOR_HPP 