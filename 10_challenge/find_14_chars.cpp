#include <system_error> // for std::error_code
#include <cstdio> // for std::printf
#include "mio.hpp"
#include <unordered_map>
#include <filesystem>
#include <optional>

using std::unordered_map;

int handle_error(const std::error_code& error);

std::optional<size_t> find_14_unique_chars_idx(mio::mmap_source& file, size_t file_size) {
    for(size_t i = 0; i < file_size - 14; i++) {
        unordered_map<char, int> counter;
        bool found = true;
        for(size_t j = i; j < i + 14; j++) {
            counter[file[j]] += 1;
        }
        for(const std::pair<const char, int>&p : counter) {
            if(p.second != 0 && p.second != 1) {
                found = false;
            }
        }
        if(found) {
            return i;
        }
    }
    return {};
}

/**
 * @brief Your optimized method for finding 14 consecutive unique chars.
 *
 * @param file This is a memory mapped file. You can treat it as a character stream/array.
 *
 * @param file_size Size of the file in bytes.
 *
 * @return Optional index of the start of the unique sequence.
 */
std::optional<size_t> find_optimized(mio::mmap_source& file, size_t file_size) {
    // for(auto ch:file) {
    //     std::printf("%c", ch);
    // }
    return {};
}

int main() {
    const auto path = "testcase.txt";
    const size_t file_size = std::filesystem::file_size(path);

    std::error_code error;
    mio::mmap_source ro_mmap;
    ro_mmap.map(path, error);
    if (error) { return handle_error(error); }
    
    std::optional<size_t> ans_idx = find_14_unique_chars_idx(ro_mmap, file_size);
    
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
