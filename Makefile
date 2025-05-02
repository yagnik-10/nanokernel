# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -fno-rtti -fno-exceptions
ASMFLAGS = -f elf64
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib

# Directories
SRC_DIR = kernel
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
ASM_SRCS = $(shell find $(SRC_DIR) -name '*.asm')
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) $(ASM_SRCS:$(SRC_DIR)/%.asm=$(OBJ_DIR)/%.o)

# Target
TARGET = $(BIN_DIR)/kernel.bin
ISO = $(BIN_DIR)/kernel.iso

# Default target
all: $(TARGET)

# Create directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/process
	mkdir -p $(OBJ_DIR)/memory
	mkdir -p $(OBJ_DIR)/interrupt
	mkdir -p $(OBJ_DIR)/sync

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile C++ sources
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile assembly sources
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	nasm $(ASMFLAGS) $< -o $@

# Link kernel
$(TARGET): $(OBJ_DIR) $(BIN_DIR) $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

# Create bootable ISO
iso: $(TARGET)
	grub-mkrescue -o $(ISO) $(BIN_DIR)

# Run in QEMU
qemu: $(TARGET)
	qemu-system-x86_64 -cdrom $(ISO)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean qemu iso 