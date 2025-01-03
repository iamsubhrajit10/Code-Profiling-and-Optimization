#include <vector>
#include <omp.h>

#define NUM_THREADS 4

using std::vector;

std::optional<size_t> find_14_unique_chars_idx(mio::mmap_source &file, size_t file_size)
{
    // for(auto ch:file) {
    //     std::printf("%c", ch);
    // }
    std::optional<size_t> result;
    int found = NUM_THREADS + 1;

#pragma omp parallel num_threads(NUM_THREADS) shared(found, result)
    {
        int thread_id = omp_get_thread_num();
        size_t chunk_size = file_size / omp_get_num_threads();
        size_t start = thread_id * chunk_size;
        size_t end = (thread_id == omp_get_num_threads() - 1) ? file_size : start + chunk_size;
        // printf("Thread %d, start: %zu, end: %zu\n", thread_id, start, end);

        // Each thread processes its own range
        for (size_t i = start; i < end - 14 && found > thread_id;)
        {
            std::vector<size_t> last_seen(26, std::numeric_limits<size_t>::max()); // Array for ASCII characters
            bool is_valid = true;
            // Populate the last_seen array for the current block
            for (size_t j = 0; j < 14; ++j)
            {
                char ch = file[i + j];
                int index = ch - 97;
                // printf("Thread %d, ch: %c, index: %d, i: %zu, j: %zu\n", thread_id, ch, index, i, j);
                if (last_seen[index] != std::numeric_limits<size_t>::max() && last_seen[index] >= i)
                {
                    // Duplicate found, skip to next block
                    // printf("Thread %d, duplicate found at index %zu\n", thread_id, i);
                    // i = last_seen[index] + 1;
                    i += 1;
                    is_valid = false;
                    // break;
                    j = 14;
                }
                last_seen[index] = i + j; // Update last seen index
            }

            // If all 14 characters are unique
            if (is_valid)
            {
// printf("Thread %d, found at index %zu\n", thread_id, i);
#pragma omp critical
                {
                    // found stores the thread id that last updated this.
                    if (thread_id < found)
                    {
                        found = thread_id;
                        result = i;
                        // break;
                        i = end;
                    }
                    else
                    {
                        // A thread smaller than the current found the answer, so this can stop.
                        // break;
                        i = end;
                    }
                }
            }
        }
    }
    if (found == NUM_THREADS + 1)
    {
        return {};
    }
    else
    {
        return result;
    }
}
