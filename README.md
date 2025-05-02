# NanoKernel

A modern, lightweight kernel implementation focusing on essential operating system concepts. NanoKernel provides a minimal but complete set of kernel features including memory management, process scheduling, and synchronization primitives.

## Author

**Yagnik Pavagadhi**
- GitHub: [github.com/yagnikpavagadhi](https://github.com/yagnikpavagadhi)

## Features

- **Memory Management**: Custom memory allocator with tracking capabilities
- **Synchronization Primitives**:
  - Reader-Writer Locks
  - Condition Variables
  - Event System
- **Multi-threading Support**: Thread-safe operations and concurrent process execution

## Building and Running

### Prerequisites
- CMake 3.10 or higher
- C++17 compatible compiler
- pthread library

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

### Running the Tests

After building, run the test program:
```bash
./build/nanokernel
```

This will demonstrate:
1. Memory allocation and deallocation
2. Event posting and handling
3. Reader-writer lock synchronization
4. Condition variable synchronization
5. Multi-threaded process execution

## Project Structure

- `kernel/`
  - `allocator.hpp/cpp`: Memory management implementation
  - `rwlock.hpp/cpp`: Reader-writer lock implementation
  - `condvar.hpp/cpp`: Condition variable implementation
  - `event.hpp/cpp`: Event system implementation
  - `main.cpp`: Test program and demonstration

## Contributing

Feel free to submit issues and enhancement requests!

## License

Copyright © 2024 Yagnik Pavagadhi. All rights reserved.
