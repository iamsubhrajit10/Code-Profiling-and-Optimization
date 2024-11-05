#include <cstdlib>
#include <list>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "../mytime.h"

using std::rand;
using std::vector;
using std::list;

const int N = 100'000;

inline int get_random() {
    return rand() % N;
}

void sorted_insert(vector<int>& vec, int val) {
    vector<int>::iterator pos = std::upper_bound(vec.begin(), vec.end(), val);
    vec.insert(pos, val);
}

void sorted_insert(list<int>& linklist, int val) {
    list<int>::iterator pos = std::upper_bound(linklist.begin(), linklist.end(), val);
    linklist.insert(pos, val);
}

int main() {
    long double time_taken;
    long double t0, t1;

    std::vector<int> vec;
    std::list<int> linklist;

    srand(20241105);
    time_taken = 0;

    t0 = cputime();
    for (int i = 0; i < N; i++) {
        sorted_insert(vec, get_random());
    }
    t1 = cputime();

    time_taken = t1 - t0;
    printf("[Vector insert] Time taken: %Lf micro seconds\n", time_taken);

    srand(20241105);
    time_taken = 0;

    t0 = cputime();
    for (int i = 0; i < N; i++) {
        sorted_insert(linklist, get_random());
    }
    t1 = cputime();

    time_taken = t1 - t0;
    printf("[List insert] Time taken: %Lf micro seconds\n", time_taken);
    return 0;
}
