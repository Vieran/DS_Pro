/* implementation of the SSTable */

#include "SSTable.h"

SSTable::SSTable() {}

// construct a sstable from a given memtable
SSTable::SSTable(MemTable *mt) {

}

SSTable::~SSTable() {

}

// use binary search to find the key
std::string SSTable::find(int64_t key) {

}

// retrun the time stamp of given sstable
uint64_t SSTable::get_timestamp(SSTable *sst) {
    return header.time_stamp;
}