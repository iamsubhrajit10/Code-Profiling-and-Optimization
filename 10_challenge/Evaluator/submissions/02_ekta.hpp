std::optional<size_t> find_14_unique_chars_idx(mio::mmap_source& file, size_t file_size) {
    if (file_size < 14) return {}; // File is too small to have 14 unique chars

    std::unordered_map<char, int> count;
    size_t c_count = 0;

    // Initialize the first window
    for(size_t i = 0; i < 14; i++) {
            count[file[i]] += 1;
            if(count[file[i]] == 1)
            	c_count++;
            if(count[file[i]] == 2)	
            	c_count--;
    }
    
    if(c_count==14)  return {};      	

    // Slide the window
    for(size_t i = 14; i < file_size; i++) {
    	char remove = file[i-14];
    	char add = file[i];
    	
   // remove the char 	
    	count[remove]--;
    	if(count[remove] == 1)
            	c_count++;
        if(count[remove] == 0)	
            	c_count--;
            	
     // add the char 	
    	count[add]++;
    	if(count[add] == 1)
            	c_count++;
        if(count[add] == 2)	
            	c_count--;
            	
       if(c_count==14)  return i-13;  	
    }
       

    return {};

}
