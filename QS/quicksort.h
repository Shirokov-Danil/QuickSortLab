#pragma once

#include <vector>
#include <cstdint>
#include <parlay/parallel.h>

template <typename T>
inline void my_swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

inline uint64_t next_random() {
    thread_local uint64_t state = 88172645463325252ULL ^ reinterpret_cast<uintptr_t>(&state);
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

template <typename T>
int partition(std::vector<T>& a, int l, int r) {
    int random_index = l + static_cast<int>(next_random() % static_cast<uint64_t>(r - l + 1));
    T v = a[random_index];
    int i = l;
    int j = r;

    while (i <= j) {
        while (a[i] < v) {
            i++;
        }
        while (a[j] > v) {
            j--;
        }
        if (i >= j) {
            break;
        }
        my_swap(a[i], a[j]);
        i++;
        j--;
    }
    return j;
}

template <typename T>
void quicksort_seq(std::vector<T>& a, int l, int r) {
    if (l < r) {
        int q = partition(a, l, r);
        quicksort_seq(a, l, q);
        quicksort_seq(a, q + 1, r);
    }
}

template <typename T>
void quicksort_seq(std::vector<T>& a) {
    if (!a.empty()) {
        quicksort_seq(a, 0, static_cast<int>(a.size()) - 1);
    }
}

template <typename T>
void quicksort_par(std::vector<T>& a, int l, int r) {
    if (l >= r) {
        return;
    }
    if (r - l < 10000) {
        quicksort_seq(a, l, r);
        return;
    }
    int q = partition(a, l, r);
    parlay::par_do(
        [&]() { quicksort_par(a, l, q); },
        [&]() { quicksort_par(a, q + 1, r); }
    );
}

template <typename T>
void quicksort_par(std::vector<T>& a) {
    if (!a.empty()) {
        quicksort_par(a, 0, static_cast<int>(a.size()) - 1);
    }
}
