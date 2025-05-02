#pragma once

#include <cstddef>

class MemoryAllocator {
public:
    MemoryAllocator();
    
    void* allocate(size_t size);
    void deallocate(void* ptr);
    size_t get_total_allocated() const;

private:
    size_t total_allocated;
}; 