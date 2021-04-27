/* definition of the SSTable */
#ifndef SSTABLE_H
#define SSTABLE_H

#include <cstdint>
#include "MemTable.h"

class SSTable {
   private:
    struct HEADER {
        uint64_t time_stamp;
        uint64_t pair_num;
        int64_t max_key;
        int64_t min_key;
    } header;

    // bloom filter
    bool *bloom_filter;
    // index
    struct INDEX {
        uint64_t key;
        uint32_t offset;
    } *index;
    // data
    std::string *data;

   public:
    SSTable();
    SSTable(MemTable *mt);
    ~SSTable();

    std::string find(int64_t key);
    uint64_t get_timestamp(SSTable *sst);
};

#endif  //  SSTABLE_H