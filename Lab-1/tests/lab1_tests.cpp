#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>


extern "C" {
#include <parent.h>
}

template<size_t N>
std::string join(const std::array<int, N> &elements, const std::string &delimiter) {
    std::string result;
    for (size_t i = 0; i < elements.size(); ++i) {
        if (i > 0) {
            result += delimiter;
        }
        result += std::to_string(elements[i]);
    }
    return result;
}

class Lab1Test : public testing::Test {
protected:
    char *fileWithInput = (char *) "input.txt";

    void TearDown() override {
        auto removeIfExists = [](const char *path) {
            if (std::filesystem::exists(path)) {
                std::filesystem::remove(path);
            }
        };

        removeIfExists(fileWithInput);
    }

    template<size_t N>
    void WriteToFile(std::array<int, N> input) {
        auto inFile = std::ofstream(fileWithInput);

        for (const auto &line: input) {
            inFile << line << '\n';
        }
    }
};


FILE *openMemoryStream(char buffer[]) {
    FILE *pFile = fmemopen(buffer, 1024, "w");
    return pFile;
}


TEST_F(Lab1Test, HandleSomePrimeNumbers) {
    std::array<int, 5> input = {
            4, 10, 5, 20, 31
    };
    WriteToFile(input);
    std::array<int, 2> expectedOutput = {
            4, 10
    };
    char buffer[1024];
    FILE *pFile = openMemoryStream(buffer);
    std::swap(stdout, pFile);

    MainFunction(fileWithInput);

    std::swap(stdout, pFile);
    fclose(pFile);
    std::string capturedOutput(buffer);
    std::replace(capturedOutput.begin(), capturedOutput.end(), '\n', ' ');
    capturedOutput.erase(capturedOutput.end() - 1, capturedOutput.end());
    EXPECT_EQ(capturedOutput, join(expectedOutput, " "));
}

TEST_F(Lab1Test, HandleAllPrimeNumbers) {
    std::array<int, 4> input = {
            3, 2, 5, 7
    };
    WriteToFile(input);
    std::array<int, 0> expectedOutput = {};
    char buffer[1024];
    FILE *pFile = openMemoryStream(buffer);
    std::swap(stdout, pFile);

    MainFunction(fileWithInput);

    std::swap(stdout, pFile);
    fclose(pFile);

    std::string capturedOutput(buffer);
    std::string expected = join(expectedOutput, " ");
    EXPECT_EQ(capturedOutput, expected);
}