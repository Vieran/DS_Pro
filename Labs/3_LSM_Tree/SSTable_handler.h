#pragma once

#include <vector>
#include "SSTable.h"

class SSTable_handler {
   private:
    struct sst_node {
        SSTable *sst;   // current sstable
        SSTable *next;  // next sstable
        SSTable *down;  // next level
        sst_node(SSTable *cur, SSTable *n, SSTable *d)
            : sst(cur), next(n), down(d) {}
    } sstable_h;

   public:
    SSTable_handler();
    ~SSTable_handler();
    void compaction();
};