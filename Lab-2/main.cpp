#include "string"
#include <chrono>
#include "tools.h"


int main(int argc, char *argv[]) {
    int THREADS_NUMBER = 1, ROUNDS_NUMBER = 10000;
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <options>\n"
                  << "Options:\n"
                  << "\t-t NUMBER\t\t Number of threads, running at the same time\n"
                  << "\t-r NUMBER\t\t Number of processed rounds"
                  << std::endl;
        return EXIT_FAILURE;
    }
    for (int i = 1; i < argc; i += 2) {
        std::string arg = argv[i];
        if (arg == "-t") {
            THREADS_NUMBER = strtol(argv[i + 1], nullptr, 10);
        } else if (arg == "-r") {
            ROUNDS_NUMBER = strtol(argv[i + 1], nullptr, 10);
        }
    }
    std::cout << "Running " << THREADS_NUMBER << " threads for " << ROUNDS_NUMBER << " rounds" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    double probability = Process(THREADS_NUMBER, ROUNDS_NUMBER);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Probability: " << probability << std::endl;

    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}
