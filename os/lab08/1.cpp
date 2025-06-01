#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <ctime>

std::vector<int> array;
int sum = 0;
std::mutex sum_mutex;

struct ThreadData {
    int start;
    int end;
};

void sum_part(ThreadData data) {
    int local_sum = 0;

    for (int i = data.start; i < data.end; ++i) {
        local_sum += array[i];
    }

    std::lock_guard<std::mutex> lock(sum_mutex);
    sum += local_sum;
}

int main() {
    int N;
    std::cout << "Enter the number of elements (N >= 16): ";
    std::cin >> N;

    if (N < 16) {
        std::cerr << "Error: N must be at least 16." << std::endl;
        return 1;
    }

    array.resize(N);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    for (int& val : array) {
        val = std::rand() % 100;
    }

    int part_size = N / 4;
    std::vector<std::thread> threads;
    std::vector<ThreadData> thread_data(4);

    for (int i = 0; i < 4; ++i) {
        thread_data[i].start = i * part_size;
        thread_data[i].end = (i == 3) ? N : (i + 1) * part_size;
        threads.emplace_back(sum_part, thread_data[i]);
    }

    std::cout << "Array elements:\n";
    for (int val : array) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    for (std::thread& t : threads) {
        t.join();
    }

    std::cout << "Total sum = " << sum << std::endl;

    return 0;
}
