#include "tools.h"
#include <gtest/gtest.h>
#include <chrono>


TEST(ThreadsTests, TestOneThread) {
    double probability = Process(1, 10000000);
    double epsilon = 1e-4;
    EXPECT_LE(abs(probability - 0.02), epsilon);
}

TEST(ThreadsTests, TestEqualityOneAndMultiThreads) {
    double probability_one_thread = Process(1, 10000000);
    double probability_several_threads = Process(4, 10000000);
    double epsilon = 1e-3;
    std::cout << probability_one_thread << " " << probability_several_threads << std::endl;
    EXPECT_NEAR(probability_one_thread, 0.02, epsilon);
    EXPECT_NEAR(probability_several_threads, 0.02, epsilon);
}

TEST(ThreadsTests, TestMultiThreadsFasterOne) {
    auto start = std::chrono::high_resolution_clock::now();

    double probability_one_thread = Process(1, 10000000);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_one_thread = end - start;
    start = std::chrono::high_resolution_clock::now();

    double probability_several_threads = Process(4, 10000000);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_multi_threads = end - start;
    EXPECT_LT(duration_multi_threads, duration_one_thread);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}