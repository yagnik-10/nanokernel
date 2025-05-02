#include <iostream>
#include <thread>
#include <vector>
#include "allocator.hpp"
#include "rwlock.hpp"
#include "condvar.hpp"
#include "event.hpp"

// Global instances of kernel components
MemoryAllocator allocator;
RWLock rwlock;
CondVar condvar;
EventSystem event_system;

void test_process(int id) {
    std::cout << "Process " << id << " started\n";

    // Test memory allocation
    void* memory = allocator.allocate(1024);
    std::cout << "Process " << id << " allocated 1KB of memory\n";

    // Test event posting
    Event event{id, 42};
    event_system.post(event);
    std::cout << "Process " << id << " posted event\n";

    // Test reader-writer lock
    rwlock.read_lock();
    std::cout << "Process " << id << " acquired read lock\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    rwlock.read_unlock();
    std::cout << "Process " << id << " released read lock\n";

    // Test condition variable
    std::cout << "Process " << id << " waiting on condition variable...\n";
    condvar.wait();
    std::cout << "Process " << id << " received notification\n";

    // Cleanup
    allocator.deallocate(memory);
    std::cout << "Process " << id << " completed\n";
}

int main() {
    std::cout << "NanoKernel Test Program\n";
    std::cout << "======================\n\n";

    // Create test processes
    std::vector<std::thread> processes;
    for (int i = 0; i < 3; i++) {
        processes.emplace_back(test_process, i);
    }

    // Give processes time to start and wait on condition variable
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // Notify all waiting processes
    std::cout << "\nMain thread: Notifying all waiting processes...\n";
    condvar.notify_all();

    // Wait for all processes to complete
    for (auto& process : processes) {
        process.join();
    }

    // Print kernel statistics
    std::cout << "\nKernel Statistics:\n";
    std::cout << "Total memory allocated: " << allocator.get_total_allocated() << " bytes\n";
    std::cout << "Active events: " << event_system.get_event_count() << "\n";

    return 0;
} 