#pragma once

#include <iostream>
#include <list>
#include "set"
#include <cmath>
#include <cstdint>
#include <cassert>


class BuddyAllocator {
public:
    BuddyAllocator(void *realMemory, size_t memory_size);

    void *alloc(size_t block_size);

    void free(void *block, size_t size);

private:
    void *realMemory;
    size_t memory_size;
    int max_order;
    std::list<void *> free_list[32];

    int get_order(size_t size);

    void *get_buddy(void *block, size_t order, int direction);

    bool is_block(void *block);


    size_t align_up(size_t size);
};