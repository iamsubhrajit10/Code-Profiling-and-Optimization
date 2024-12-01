#include <system_error> // for std::error_code
#include <cstdio> // for std::printf
#include <unordered_map>
#include <filesystem>
#include <optional>
#include "headers/mio.hpp"
#include "headers/find14.hpp"
extern "C" {
    #include "headers/mytime.h"
}

using std::unordered_map;

int handle_error(const std::error_code& error);

int main(int argc, char* argv[]) {
    long double time_taken;
    struct timespec start, end;
    long double t0, t1;

    const auto path = argv[1];
    const size_t file_size = std::filesystem::file_size(path);

    std::error_code error;
    mio::mmap_source ro_mmap;
    ro_mmap.map(path, error);
    if (error) { return handle_error(error); }
    
    // We iter over the file to load it in memory
    size_t throwaway = 0;
    for(auto ch: ro_mmap) {
        throwaway++;
    }
    std::printf("Read the file till idx: %zu\n", throwaway);
    
    
    time_taken = 0;
    start = get_timespec();
    std::optional<size_t> ans_idx = find_14_unique_chars_idx(ro_mmap, file_size);
    end = get_timespec();
    time_taken = diff_timespec_us(start, end);
    
    std::printf("Time taken: %Lf\n", time_taken);
    
    if(ans_idx) {
        std::printf("Found 14 unique chars at index %zu\n", ans_idx.value());
    } else {
        std::printf("Could not find 14 unique chars.");
    }
    
    ro_mmap.unmap();

    return 0;
}

int handle_error(const std::error_code& error)
{
    const auto& errmsg = error.message();
    std::printf("error mapping file: %s, exiting...\n", errmsg.c_str());
    return error.value();
}
