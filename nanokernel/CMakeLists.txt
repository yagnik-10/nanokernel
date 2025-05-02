cmake_minimum_required(VERSION 3.10)

# Project metadata
project(nanokernel 
    VERSION 1.0.0
    DESCRIPTION "A modern, lightweight kernel implementation"
    HOMEPAGE_URL "https://github.com/yagnikpavagadhi/nanokernel"
    LANGUAGES CXX
)

# Set author information
set(AUTHOR_NAME "Yagnik Pavagadhi")
set(AUTHOR_EMAIL "yagnik.pavagadhi06@gmail.com")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)

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
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link with pthread
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/kernel) 