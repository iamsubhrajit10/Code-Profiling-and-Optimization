#include <cstdlib>
#include <stdio.h>
#include <unordered_map>
#include <vector>
#include "../mytime.h"

using std::rand;
using std::vector;
using std::unordered_map;

const int N = 1'000'000;

inline int get_random() {
    return rand() % N;
}

int main() {
    long double time_taken;
    long double t0, t1;

    std::vector<int> vec(N);
    std::unordered_map<int, int> map;
    std::unordered_map<int, int> map2;
    for (int i = 0; i < N; i++) { // pre-fill map2
        map2[i] = 0;
    }

    srand(20241105);
    time_taken = 0;
    t0 = cputime();
    for (int i = 0; i < N; i++) {
        int val = get_random();
        vec[val] += 1;
    }
    t1 = cputime();

    time_taken = t1 - t0;
    printf("[Vector direct access] Time taken: %Lf micro seconds\n", time_taken);

    srand(20241105);
    time_taken = 0;
    t0 = cputime();
    for (int i = 0; i < N; i++) {
        int val = get_random();
        if(!map.contains(val)) {
            map[val] = 0;
        }
        map[val] += 1;
    }
    t1 = cputime();

    time_taken = t1 - t0;
    printf("[Map check + fill] Time taken: %Lf micro seconds\n", time_taken);

    srand(20241105);
    time_taken = 0;
    t0 = cputime();
    for (int i = 0; i < N; i++) {
        int val = get_random();
        map2[val] += 1;
    }
    t1 = cputime();

    time_taken = t1 - t0;
    printf("[Map direct] Time taken: %Lf micro seconds\n", time_taken);
    return 0;
}
