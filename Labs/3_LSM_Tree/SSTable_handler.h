#pragma once

#include <vector>
#include "SSTable.h"

class SSTable_handler {
   private:
    // a list to hold the sst handler
    // each element refer to the first sst_node(which is a sstable) on this level
    std::vector<std::vector<SSTable*>> sst_handler_list;
    uint64_t t_stamp;
    std::string dir_name;

   public:
    SSTable_handler();
    ~SSTable_handler();
    bool exist(uint64_t key);
    std::string get(uint64_t key);
    void compaction();
    void to_sst(MemTable &memt);
    void read_file(const std::string &dir);
    void clear();
};