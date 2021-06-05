/* implementation of the SSTable */

#include "SSTable.h"
#include "MurmurHash3.h"

SSTable::SSTable() {}

// construct a sstable from a given memtable
SSTable::SSTable(MemTable *mt) {
}

SSTable::~SSTable() {
}

// setup a bloom filter for a new added key
void SSTable::set_bf(int64_t key) {
    unsigned int hash[4] = {0};
    MurmurHash3_x64_128(&key, sizeof(key), 1, hash);

    bloom_filter[hash[0]] = true;
    bloom_filter[hash[1]] = true;
    bloom_filter[hash[2]] = true;
    bloom_filter[hash[3]] = true;
}

// use bloom filter to judge if the node exist
// false: cannot exist; true: probably exist
bool SSTable::exist(int64_t key) {
    unsigned int hash[4] = {0};
    MurmurHash3_x64_128(&key, sizeof(key), 1, hash);

    if (bloom_filter[hash[0]] && bloom_filter[hash[1]] && bloom_filter[hash[2]] && bloom_filter[hash[3]])
        return true;
    return false;
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