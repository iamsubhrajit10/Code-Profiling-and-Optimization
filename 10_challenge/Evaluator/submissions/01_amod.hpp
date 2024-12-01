std::optional<size_t> find_14_unique_chars_idx(mio::mmap_source& file, size_t file_size) 
{
    #define UNLIKELY(x) __builtin_expect(!!(x), 0)

    int counters[26] = {0};

    for(size_t j = 0; j < 14; j++) 
    {
        counters[file[j] - 97] += 1;
    }

    for(size_t i = 13; i < file_size; i++) 
    {
        bool found = true;

        for(int k = 0; k < 26; k++) 
        {
            if(counters[k] > 1)
            {
                found = false;
                break;
            }
        }

        if(UNLIKELY(found))
        {
            return i-13;
        }

        counters[file[i-13] - 97] -= 1;
        counters[file[i+1] - 97] += 1;

    }
    return {};
}