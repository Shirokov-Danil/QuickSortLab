#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include "quicksort.h"

int main() {
    const size_t N = 100000000;
    const int RUNS = 5;
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> val_dist(-1000000000, 1000000000);
    std::vector<int> base_arr(N);
    std::vector<int> work_arr(N);
    double total_seq = 0.0;
    double total_par = 0.0;

    std::cout << "Parlay worker threads: " << parlay::num_workers() << "\n\n";

    for (int run = 1; run <= RUNS; ++run) {
        for (size_t i = 0; i < N; ++i) {
            base_arr[i] = val_dist(gen);
        }

        work_arr = base_arr;
        auto t1 = std::chrono::high_resolution_clock::now();
        quicksort_seq(work_arr);
        auto t2 = std::chrono::high_resolution_clock::now();
        double time_seq = std::chrono::duration<double>(t2 - t1).count();
        bool ok_seq = std::is_sorted(work_arr.begin(), work_arr.end());
        total_seq += time_seq;

        work_arr = base_arr;
        auto t3 = std::chrono::high_resolution_clock::now();
        quicksort_par(work_arr);
        auto t4 = std::chrono::high_resolution_clock::now();
        double time_par = std::chrono::duration<double>(t4 - t3).count();
        bool ok_par = std::is_sorted(work_arr.begin(), work_arr.end());
        total_par += time_par;

        std::cout << "Run #" << run
            << " | Seq: " << time_seq << "s (" << (ok_seq ? "OK" : "WA") << ")"
            << " | Par: " << time_par << "s (" << (ok_par ? "OK" : "WA") << ")\n";
    }

    double avg_seq = total_seq / RUNS;
    double avg_par = total_par / RUNS;
    std::cout << "\nAverage Seq: " << avg_seq << "s\n";
    std::cout << "Average Par: " << avg_par << "s\n";
    std::cout << "Average Speedup: " << (avg_seq / avg_par) << "x\n";

    return 0;
}
