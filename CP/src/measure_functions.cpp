#include "measure_functions.h"

#include "chrono"
#include "iostream"

double calculateUsageFactor(Allocator& allocator) {
    size_t totalMemory = allocator.getTotalMemorySize();
    size_t freeMemory = allocator.getFreeMemorySize();
    return static_cast<double>(totalMemory - freeMemory) / totalMemory;
}

// Функция для измерения скорости выделения
double measureAllocationSpeed(Allocator& allocator, size_t numAllocations, size_t blockSize) {
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < numAllocations; ++i) {
        void* block = allocator.alloc(blockSize);
        if (!block) {
            std::cerr << "Allocation failed!" << std::endl;
            return -1;
        }
        auto startFree = std::chrono::high_resolution_clock::now();
        allocator.free(block);
        auto endFree = std::chrono::high_resolution_clock::now();
        start += (endFree - startFree);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count() / numAllocations;
}

// Функция для измерения скорости освобождения
double measureDeallocationSpeed(Allocator& allocator, std::vector<void*>& blocks) {
    auto start = std::chrono::high_resolution_clock::now();
    for (void* block : blocks) {
        allocator.free(block);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count() / blocks.size();
}

// Функция для оценки простоты использования
void evaluateEaseOfUse(Allocator& allocator) {
    // Пример оценки
    std::cout << "Ease of use evaluation for " << typeid(allocator).name() << std::endl;
}