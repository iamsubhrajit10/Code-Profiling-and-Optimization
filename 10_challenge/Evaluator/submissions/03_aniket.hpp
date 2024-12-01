#include <vector>

std::optional<size_t> find_14_unique_chars_idx(mio::mmap_source& file, size_t file_size) {
    if (file_size < 14) return {};

    std::vector<int> char_count(256, 0); // ASCII character set size
    size_t unique_count = 0;

    // Initialize the first window of 14 characters
    for (size_t i = 0; i < 14; ++i) {
        if (char_count[file[i]] == 0) ++unique_count;
        char_count[file[i]]++;
    }

    // Check if the first window is valid
    if (unique_count == 14) return 0;

    // Slide the window across the file
    for (size_t i = 14; i < file_size; ++i) {
        // Add the new character (right end of the window)
        if (char_count[file[i]] == 0) ++unique_count;
        char_count[file[i]]++;

        // Remove the old character (left end of the window)
        char_count[file[i - 14]]--;
        if (char_count[file[i - 14]] == 0) --unique_count;

        // Check if current window has 14 unique characters
        if (unique_count == 14) return i - 13;
    }

    return {};
}
