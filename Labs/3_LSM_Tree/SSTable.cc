/* implementation of the SSTable */

#include "SSTable.h"

SSTable::SSTable() {}

// construct a sstable from a given memtable
SSTable::SSTable(MemTable *mt) {

}

SSTable::~SSTable() {

}

// use bloom filter to judge if the node exist
std::string SSTable::exist(int64_t key) {

}

// use binary search to quickly find the key
// fetch the value from the file in the disk
std::string SSTable::get(int64_t key) {

}

// retrun the time stamp of given sstable
uint64_t SSTable::get_timestamp() {
    return header.time_stamp;
}

// write a sstable to the disk
void SSTable::write_file() {

}