#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>



extern "C" {
#include <parent.h>
}


TEST(MockCout, test_1) {
    char *fileWithInput = (char *) "input.txt";
    std::array<int, 5> input = {
            4, 10, 5, 20, 31
    };

    std::array<int, 2> expectedOutput = {
            4, 10
    };

    {
        auto inFile = std::ofstream(fileWithInput);

        for (const auto &line: input) {
            inFile << line << '\n';
        }
    }
    char mybuffer[1024];
    FILE * pFile = fmemopen(mybuffer, 1024, "w");
    std::swap(stdout, pFile);

    MainFunction(fileWithInput);

    std::swap(stdout, pFile);
    fclose(pFile);
    std::string capturedOutput(mybuffer);
    std::replace(capturedOutput.begin(), capturedOutput.end(), '\n', ' ');
    capturedOutput.erase(capturedOutput.end()-1, capturedOutput.end());
    ASSERT_EQ(capturedOutput, "4 10");


    auto removeIfExists = [](const char *path) {
        if (std::filesystem::exists(path)) {
            std::filesystem::remove(path);
        }
    };

    removeIfExists(fileWithInput);
}