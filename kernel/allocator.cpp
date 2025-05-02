#include "allocator.hpp"
#include <cstdlib>
#include <iostream>

MemoryAllocator::MemoryAllocator() : total_allocated(0) {}

void* MemoryAllocator::allocate(size_t size) {
    void* ptr = std::malloc(size);
    if (ptr) {
        total_allocated += size;
    }
    return ptr;
}

void MemoryAllocator::deallocate(void* ptr) {
    if (ptr) {
        std::free(ptr);
    }
}

size_t MemoryAllocator::get_total_allocated() const {
    return total_allocated;
} 