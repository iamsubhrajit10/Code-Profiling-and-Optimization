#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "../mytime.h"

using std::rand;

const int N = 1'000'000;

inline int get_random() {
    return rand() % N;
}

int main() {
    long double time_taken;
    long double t0, t1;

    std::stringstream ss;

    srand(20241105);
    time_taken = 0;

    t0 = cputime();
    for (int i = 0; i < N; i++) {
        std::cerr << get_random();
    }
    t1 = cputime();

    time_taken = t1 - t0;
    printf("[print per iteration] Time taken: %Lf micro seconds\n", time_taken);

    srand(20241105);
    time_taken = 0;

    t0 = cputime();
    for (int i = 0; i < N; i++) {
        ss << get_random();
    }
    std::cerr << ss.str();
    t1 = cputime();

    time_taken = t1 - t0;
    printf("[print once] Time taken: %Lf micro seconds\n", time_taken);
    return 0;
}
