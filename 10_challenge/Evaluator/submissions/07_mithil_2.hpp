#include <vector>
#include <omp.h>

#define NUM_THREADS 4

using std::vector;

std::optional<size_t> find_14_unique_chars_idx(mio::mmap_source &file, size_t file_size)
{
    std::optional<size_t> result;
    int found = NUM_THREADS + 1;

#pragma omp parallel num_threads(NUM_THREADS) shared(found, result)
    {
        int thread_id = omp_get_thread_num();
        size_t chunk_size = file_size / omp_get_num_threads();
        size_t start = thread_id * chunk_size;
        size_t end = (thread_id == omp_get_num_threads() - 1) ? file_size : start + chunk_size;
        // printf("Thread %d, start: %zu, end: %zu\n", thread_id, start, end);

        for (size_t i = start; i < end && found > thread_id; i++)
        {
            int character = file[i];
            if (character == 110)
            {
#pragma omp critical
                {
                    // found stores the thread id that last updated this.
                    if (thread_id < found)
                    {
                        found = thread_id;
                        result = i - 13;
                        // break;
                    }
                }
                if (thread_id < found)
                {
                    i = end;
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
