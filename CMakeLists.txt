cmake_minimum_required(VERSION 3.10)
project(NanoKernel VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    kernel/main.cpp
    kernel/allocator.cpp
    kernel/rwlock.cpp
    kernel/condvar.cpp
    kernel/event.cpp
)

# Add header files
set(HEADERS
    kernel/allocator.hpp
    kernel/rwlock.hpp
    kernel/condvar.hpp
    kernel/event.hpp
)

# Create executable
add_executable(nanokernel ${SOURCES} ${HEADERS})

# Link with pthread
find_package(Threads REQUIRED)
target_link_libraries(nanokernel PRIVATE Threads::Threads)

# Set include directories
target_include_directories(nanokernel PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Set compiler flags
target_compile_options(nanokernel PRIVATE
    -Wall
    -Wextra
    -O2
) 