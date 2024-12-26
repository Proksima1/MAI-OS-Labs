#pragma once
#include "allocators.h"


double calculateUsageFactor(Allocator& allocator);

// Функция для измерения скорости выделения
double measureAllocationSpeed(Allocator& allocator, size_t numAllocations, size_t blockSize);

// Функция для измерения скорости освобождения
double measureDeallocationSpeed(Allocator& allocator, std::vector<void*>& blocks);

// Функция для оценки простоты использования
void evaluateEaseOfUse(Allocator& allocator);