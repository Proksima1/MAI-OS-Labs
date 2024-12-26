#include "cmath"
#include "vector"
#include "allocators.h"
#include "iostream"
#include <cstdint>
#include "algorithm"

size_t getNearestPowerOfTwo(size_t size) {
    if (size == 0) return 1;
    --size;
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    size |= size >> 32;
    return size + 1;
}

BuddyAllocator::BuddyAllocator(void *realMemory, size_t memory_size)
        : realMemory(realMemory), memory_size(memory_size) {
    max_order = static_cast<int>(std::ceil(std::log2(memory_size)));
    for (int i = 0; i < max_order; ++i) {
        free_list[i].clear();
    }
    void *block = realMemory;
    free_list[max_order - 1].push_back(block);
    allocated = 0;
}

void *BuddyAllocator::alloc(size_t block_size) {
    size_t aligned_size = getNearestPowerOfTwo(block_size);
    int order = get_order(aligned_size);
    if (order >= max_order)
        return nullptr;

    if (free_list[order].empty()) {
        for (size_t o = order + 1; o < max_order; ++o) {
            if (!free_list[o].empty()) {
                void *block = free_list[o].front();
                free_list[o].pop_front();
                size_t half_order = o - 1;
                void *buddy = get_buddy(block, half_order, 1);
                mapBlocks[half_order].insert({block, buddy});
                mapBlocks[half_order].insert({buddy, block});
                free_list[half_order].push_back(block);
                free_list[half_order].push_back(buddy);
                return alloc(block_size);
            }
        }
        return nullptr;
    } else {
        void *block = free_list[order].front();
        free_list[order].pop_front();
        allocated += aligned_size;
        return block;
    }
}

void BuddyAllocator::mergeBlocks(void *block, int order) {
    while (order < max_order - 1) {
        void *buddy = get_buddy_map(block, order);
        auto it_buddy = std::find(free_list[order].begin(), free_list[order].end(), buddy);
        auto it_block = std::find(free_list[order].begin(), free_list[order].end(), block);
        if (it_buddy != free_list[order].end()) {
            free_list[order].erase(it_buddy);
            free_list[order].erase(it_block);
            order++;
            if (buddy < block) {
                std::swap(buddy, block);
            }
            free_list[order].push_back(block);
        } else {
            break;
        }
    }
}

void BuddyAllocator::free(void *block, size_t size) {
    if (!is_block(block)) { return; }

    int order = get_order(size);
    free_list[order].push_back(block);
    allocated -= getNearestPowerOfTwo(size);
    mergeBlocks(block, order);
}

int BuddyAllocator::get_order(size_t size) {
    if (size == 0)
        return 0;
    size_t aligned_size = align_up(size);
    int order = static_cast<int>(std::ceil(std::log2(aligned_size)));
    if (order < 5) {
        order = 5;
    }
    return order;
}

void *BuddyAllocator::get_buddy(void *block, size_t order, int direction) {
    size_t block_addr = reinterpret_cast<size_t>(block);
    size_t buddy_addr = block_addr + (1 << order) * direction;
    return reinterpret_cast<void *>(buddy_addr);
}

void *BuddyAllocator::get_buddy_map(void *block, size_t order) {
    return mapBlocks[order][block];
}

bool BuddyAllocator::is_block(void *block) {
    size_t block_addr = reinterpret_cast<size_t>(block);
    size_t start = reinterpret_cast<size_t>(realMemory);
    size_t finish = reinterpret_cast<size_t>(realMemory) + memory_size;
    return block_addr >= start && block_addr < finish;
}

size_t BuddyAllocator::align_up(size_t size) {
    const size_t alignment = 1;
    return (size + (alignment - 1)) & ~((alignment) - 1);
}


PowerOfTwoAllocator::PowerOfTwoAllocator(void *realMemory, size_t memory_size) : memory(realMemory),
                                                                                 memorySize(memory_size) {
    if (!memory || memory_size == 0) {
        throw std::invalid_argument("Invalid memory or size");
    }
    pos = 0;
}

void *PowerOfTwoAllocator::alloc(size_t block_size) {
    size_t requiredSize = getNearestPowerOfTwo(block_size);
    auto foundBlock = std::find_if(freeBlocks.begin(), freeBlocks.end(), [requiredSize](const MemoryBlock *block) {
        return block->size == requiredSize;
    });
    if (foundBlock != freeBlocks.end()) {
        blocks.push_back(*foundBlock);
        freeBlocks.erase(foundBlock);
        return (*foundBlock)->address;
    } else {
        if (memorySize - pos >= block_size) {
            auto newBlock = new MemoryBlock(reinterpret_cast<void *>(reinterpret_cast<size_t>(memory) + pos),
                                            requiredSize);
            blocks.push_back(newBlock);
            pos += requiredSize;
            return newBlock->address;
        }
    }
    return nullptr;

}

bool PowerOfTwoAllocator::isBlock(void *block) {
    size_t blockAddress = reinterpret_cast<size_t>(block);
    size_t start = reinterpret_cast<size_t>(memory);
    size_t end = reinterpret_cast<size_t>(memory) + memorySize;
    return blockAddress >= start && blockAddress < end;
}

void PowerOfTwoAllocator::free(void *block, size_t block_size = -1) {
    if (!isBlock(block)) return;
    auto it = findBlock(block);

    if (it != blocks.end()) {
        freeBlocks.push_back(*it);
        blocks.erase(it);
    }
    return;
}

std::vector<MemoryBlock *>::iterator PowerOfTwoAllocator::findBlock(void *address) {
    return std::find_if(blocks.begin(), blocks.end(), [address](const MemoryBlock *block) {
        return block->address == address;
    });
}
