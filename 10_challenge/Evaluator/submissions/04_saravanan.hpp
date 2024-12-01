#include <vector>

using std::vector;

std::optional<size_t> find_14_unique_chars_idx(mio::mmap_source& file, size_t file_size) {
    for(size_t i = 0; i < file_size - 14; i++) {
            vector<bool> cnt(256,false);
            bool found = true;
            for(size_t j = i; j < i + 14; j++) {
                int ele = (int)file[j];
                if(!cnt[ele])
                cnt[ele] = true;
                else{
                    found = !found;
                    break;
                }
            }
            if(found) {
                return i;
            }
        }
    return {};
}
