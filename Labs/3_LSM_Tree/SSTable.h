/* definition of the SSTable */
#pragma once

#include <cstdint>
#include "MemTable.h"

struct HEADER {
    uint64_t time_stamp;
    uint64_t pair_num;
    int64_t max_key;
    int64_t min_key;
};

struct INDEX {
    uint64_t key;
    uint32_t offset;
};

class SSTable {
   private:
    HEADER header;       // header
    bool *bloom_filter;  // bloom filter
    INDEX *index;        // index
    uint32_t level;      // level of this sstable
    uint32_t position;   // index of sstable in the level

   public:
    SSTable();
    SSTable(MemTable *mt);
    ~SSTable();
    std::string exist(int64_t key);
    std::string get(int64_t key);
    uint64_t get_timestamp();
    void write_file();
};