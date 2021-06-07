/* definition of the SSTable */
#pragma once

#include <cstdint>
#include "MemTable.h"

struct HEADER {
    uint64_t time_stamp;
    uint64_t pair_num;
    uint64_t min_key;
    uint64_t max_key;
};

struct INDEX {
    uint64_t key;
    uint32_t offset;  // refer to the start index of the key-value
};

class SSTable {
  private:
    HEADER header;       // header
    bool bloom_filter[10240];  // bloom filter
    INDEX *index;        // index
    std::string filename;
    uint32_t total_size;

    void set_bf(uint64_t key);       // set up a bloom filter

  public:
    SSTable();
    SSTable(MemTable &memt, uint64_t &t_stamp, uint64_t level = 0);
    ~SSTable();
    bool exist(uint64_t key);
    std::string get(uint64_t key);
    uint64_t get_timestamp();
    void clear();
};