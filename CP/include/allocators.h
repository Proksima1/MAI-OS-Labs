#pragma once

#include <iostream>
#include <list>
#include "set"
#include <cmath>
#include <cstdint>
#include <cassert>
#include <cstddef>
#include <map>
#include "vector"

struct MemoryBlock {
    void *address;
    size_t size;
};

struct Stats {
    size_t total_memory;
    size_t used_memory;
    size_t free_memory;
    double utilization;
};

class Allocator {
public:
    virtual void *alloc(size_t block_size) = 0;

    virtual void free(void *block, size_t block_size = -1) = 0;

    virtual size_t getTotalMemorySize() const = 0;

    virtual size_t getFreeMemorySize() = 0;

    Stats getStats() {
        Stats stats = {0};
        stats.total_memory = getTotalMemorySize();
        stats.free_memory = getFreeMemorySize();
        stats.used_memory = stats.total_memory - stats.free_memory;
        if (stats.total_memory > 0) {
            stats.utilization = (double) stats.used_memory / (double) stats.total_memory;
        } else {
            stats.utilization = 0.0;
        }
        return stats;
    }
};

class BuddyAllocator : public Allocator {
public:
    BuddyAllocator(void *realMemory, size_t memory_size);

    void *alloc(size_t block_size);

    void free(void *block, size_t size) override;

private:
    void *realMemory;
    size_t memory_size;
    int max_order;
    std::list<void *> free_list[32];
    std::map<void*, void*> mapBlocks[32];
    size_t allocated;

    int get_order(size_t size);

    void *get_buddy(void *block, size_t order, int direction);

    bool is_block(void *block);

    size_t align_up(size_t size);

    void mergeBlocks(void *block, int order);

    void *get_buddy_map(void *block, size_t order);

    size_t getTotalMemorySize() const override {
        return memory_size;
    }

    size_t getFreeMemorySize() override{
        return memory_size - allocated;
    }
};

class PowerOfTwoAllocator : public Allocator {
public:
    PowerOfTwoAllocator(void *realMemory, size_t memory_size);

    void *alloc(size_t block_size);

    void free(void *block, size_t block_size) override;

    bool isBlock(void *block);

    std::vector<MemoryBlock *>::iterator findBlock(void *address);

    size_t getTotalMemorySize() const override {
        return memorySize;
    }

    size_t getFreeMemorySize() override {
        size_t allocatedSize = 0;
        for (auto & block : blocks) {
            allocatedSize += block->size;
        }
        return memorySize - allocatedSize;
    }

private:
    void *memory;
    std::vector<MemoryBlock *> blocks;
    std::vector<MemoryBlock *> freeBlocks;
    size_t memorySize;
    size_t pos;
};
