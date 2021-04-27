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
    char *bloom_filter;  // TODO: allocate memory for it in construction func
    // index
    // data

   public:
    SSTable();
    SSTable(MemTable *mt);
    ~SSTable();

    std::string find(int64_t key);
    uint64_t get_timestamp(SSTable *sst);
};

#endif  //  SSTABLE_H