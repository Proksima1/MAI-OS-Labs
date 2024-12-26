#include "buddy.h"


BuddyAllocator::BuddyAllocator(void *realMemory, size_t memory_size)
        : realMemory(realMemory), memory_size(memory_size) {
    max_order = static_cast<int>(std::ceil(std::log2(memory_size)));
    for (int i = 0; i < max_order; ++i) {
        free_list[i].clear();
    }
    void *block = realMemory;
    std::cout << "start memory " << realMemory << " " << reinterpret_cast<size_t>(realMemory) << std::endl;
    std::cout << "end memory " << reinterpret_cast<void *>(reinterpret_cast<size_t>(realMemory) + memory_size )
              << " " << reinterpret_cast<size_t>(realMemory) + memory_size << std::endl;
    free_list[max_order - 1].push_back(block);
}

void *BuddyAllocator::alloc(size_t block_size) {
    size_t aligned_size = align_up(block_size);
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
                free_list[half_order].push_back(block);
                free_list[half_order].push_back(buddy);
                return alloc(block_size);
            }
        }
        return nullptr;
    } else {
        void *block = free_list[order].front();
        free_list[order].pop_front();

        return block;
    }
}

void BuddyAllocator::free(void *block, size_t size) {
    if (!is_block(block)) { return; }

    int order = get_order(size);
    free_list[order].push_back(block);

    while (order < max_order - 1) {
        void *buddy = get_buddy(block, order, 1);
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
            buddy = get_buddy(block, order, -1);
            it_buddy = std::find(free_list[order].begin(), free_list[order].end(), buddy);
            if (it_buddy != free_list[order].end()){
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
}

int BuddyAllocator::get_order(size_t size) {
    if (size == 0)
        return 0;
    size_t aligned_size = align_up(size);
    return static_cast<int>(std::ceil(std::log2(aligned_size)));
}

void *BuddyAllocator::get_buddy(void *block, size_t order, int direction) {
    size_t block_addr = reinterpret_cast<size_t>(block);
    size_t buddy_addr = block_addr + (1 << order) * direction;
    return reinterpret_cast<void *>(buddy_addr);
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