#include "tools.h"
#include "random"
#include "pthread.h"

const int PACK_SIZE = 52;

pthread_mutex_t mutex;


void *MakeOneRound(void *arg) {
    auto *data = (ThreadData *) arg;
    int local_count = 0;
    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<int> dist(0, 51);
    for (int i = 0; i < data->round_numbers; ++i) {
        int firstCard = dist(rng), secondCard = dist(rng);
        if ((*data->pack)[firstCard] == (*data->pack)[secondCard]) {
            local_count++;
        }
    }
    pthread_mutex_lock(&mutex);
    (*data->positive_results) += local_count;
    pthread_mutex_unlock(&mutex);

    return nullptr;
}

std::vector<std::string> MakePack() {
    std::vector<std::string> cards;
    cards.reserve(PACK_SIZE);
    for (int i = 0; i < PACK_SIZE; ++i) {
        cards.push_back("Card " + std::to_string(i % (PACK_SIZE - 1) + 1));
    }
    return cards;
}

double Process(int THREADS_NUMBER, int ROUNDS_NUMBER) {
    pthread_t threads[THREADS_NUMBER];
    std::vector<std::string> pack = MakePack();
    int positive_results = 0;
    pthread_mutex_init(&mutex, nullptr);
    int rounds_per_thread = ROUNDS_NUMBER / THREADS_NUMBER;
    int rounds_left = ROUNDS_NUMBER - rounds_per_thread * THREADS_NUMBER;
    for (int i = 0; i < THREADS_NUMBER; ++i) {
        ThreadData d = {&pack, &positive_results, rounds_per_thread + rounds_left};
        rounds_left = 0;
        pthread_create(&threads[i], nullptr, MakeOneRound, &d);
    }
    for (int i = 0; i < THREADS_NUMBER; i++) {
        pthread_join(threads[i], nullptr);
    }

    pthread_mutex_destroy(&mutex);
    return static_cast<double>(positive_results) / ROUNDS_NUMBER;
}