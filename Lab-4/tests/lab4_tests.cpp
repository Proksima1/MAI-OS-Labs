#include <gtest/gtest.h>
//#include <fstream>
#include "gcd.h"
#include "pi.h"


TEST(GCDTest, Euclid) {
    EXPECT_EQ(GCD_Euclid(10, 5), 5);
    EXPECT_EQ(GCD_Euclid(25, 15), 5);
    EXPECT_EQ(GCD_Euclid(100, 25), 25);
    EXPECT_EQ(GCD_Euclid(17, 13), 1);
}

TEST(GCDTest, Naive) {
    EXPECT_EQ(GCD_Naive(10, 5), 5);
    EXPECT_EQ(GCD_Naive(25, 15), 5);
    EXPECT_EQ(GCD_Naive(100, 25), 25);
    EXPECT_EQ(GCD_Naive(17, 13), 1);
}

TEST(PiTest, Leibniz) {
    EXPECT_NEAR(PiLeibniz(5), 3.339, 1e-3);
    EXPECT_NEAR(PiLeibniz(100), 3.131, 1e-3);
    EXPECT_NEAR(PiLeibniz(1000000), 3.1415, 1e-4);


}

TEST(PITest, Wallis) {
    EXPECT_NEAR(PiWallis(5), 3.002, 1e-3);
    EXPECT_NEAR(PiWallis(100), 3.133, 1e-3);
    EXPECT_NEAR(PiWallis(1000000), 3.1415, 1e-4);

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}