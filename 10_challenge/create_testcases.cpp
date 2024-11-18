#include <fstream>
#include <system_error> // for std::error_code
#include <cstdio> // for std::printf
#include <iostream>
#include "mio.hpp"

using std::rand;

const int N = 100'000;
const int SET_SIZE = 13;

int handle_error(const std::error_code& error);
void allocate_file(const std::string& path, const int size);

inline size_t get_random_set_idx() {
    return rand() % SET_SIZE;
}

inline size_t get_random_ans_idx() {
    return rand() % (N - SET_SIZE - 1);
}

int main() {
    const auto path = "testcase.txt";
    allocate_file(path, N);

    std::error_code error;
    mio::mmap_sink rw_mmap = mio::make_mmap_sink(
            path, 0, mio::map_entire_file, error);
    if (error) { return handle_error(error); }
    
    srand(20241118);
    char charset[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm'};

    for(auto& b: rw_mmap) {
        b = charset[get_random_set_idx()];
    }

    const size_t answer_idx = get_random_ans_idx();
    // std::cout << answer_idx << std::endl;
    // const size_t answer_idx = 0;
    for(size_t i = answer_idx; i < answer_idx + SET_SIZE; i++) {
        char c = charset[i % SET_SIZE];
        // printf("%zu %c\n", i, c);
        rw_mmap[i] = c;
    }
    rw_mmap[answer_idx + SET_SIZE] = 'n';

    rw_mmap.sync(error);
    if (error) { return handle_error(error); }
    rw_mmap.unmap();

    return 0;
}

void allocate_file(const std::string& path, const int size)
{
    std::ofstream file(path);
    std::string s(size, '0');
    file << s;
}

int handle_error(const std::error_code& error)
{
    const auto& errmsg = error.message();
    std::printf("error mapping file: %s, exiting...\n", errmsg.c_str());
    return error.value();
}
