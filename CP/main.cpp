#include "allocators.h"
#include <iostream>
#include "vector"
#include "chrono"

const int MEMORY_SIZE = 1 << 21;
const int TEST_ITERATIONS = 1000;

void *alloc(Allocator *allocator, size_t block_size) {
    return allocator->alloc(block_size);
}

void freeAlloc(Allocator *allocator, std::pair<void *, size_t> block) {
    allocator->free(block.first, block.second);
}


void run_tests(Allocator *allocator, const char *name) {
    std::vector<std::pair<void *, size_t>> blocks(TEST_ITERATIONS);
    size_t total_requested = 0;
    std::cout << std::endl << "--- Started " << name << " ---";

    // тест выделения
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        size_t size = rand() % 1024 + 1;
        total_requested += size;
        blocks[i] = {alloc(allocator, size), size};
        if (blocks[i].first == NULL) {
            printf("Failed to allocate block of size %zu num %d\n", size, i);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> alloc_time = end - start;

    Stats stats_after_alloc = allocator->getStats();

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        freeAlloc(allocator, blocks[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> free_time = end - start;

    Stats stats_after_free = allocator->getStats();
    std::cout << "\nResults for " << name << ":\n";
    std::cout << "Allocation time " << alloc_time.count() << " seconds\n";
    std::cout << "Deallocation time: " << free_time.count() << " seconds\n";
    std::cout << "Total memory: " << stats_after_alloc.total_memory << " bytes\n";
    std::cout << "Total requested: " << total_requested << " bytes\n";
    std::cout << "Used memory after alloc: " << stats_after_alloc.used_memory << " bytes\n";
    std::cout << "Free memory after alloc: " << stats_after_alloc.free_memory << " bytes\n";
    std::cout << "Memory usage: " << round(((double) total_requested / stats_after_alloc.used_memory) * 10000) / 100
              << "%\n";
    std::cout << "Memory utilization: " << round(stats_after_alloc.utilization * 10000) / 100 << "%\n";
    std::cout << "Used memory after free: " << stats_after_free.used_memory << " bytes\n";
    std::cout << "Free memory after free: " << stats_after_free.free_memory << " bytes\n";
}


int main() {
    void *memory1 = malloc(MEMORY_SIZE);
    void *memory2 = malloc(MEMORY_SIZE);
    BuddyAllocator allocator1 = BuddyAllocator(memory1, MEMORY_SIZE);
    PowerOfTwoAllocator allocator2 = PowerOfTwoAllocator(memory2, MEMORY_SIZE);
    run_tests(&allocator1, "Buddy allocator");
    run_tests(&allocator2, "Power of 2 allocator");

    free(memory1);
    free(memory2);
    return 0;
}