/* definition of the MemTable */
#pragma once

#include <cstdint>
#include <string>
//#define MAXSIZE 2086880
#define MAXSIZE 60

struct NODE {
    NODE *right, *down;
    uint64_t key;
    std::string val;
    NODE(NODE *right, NODE *down, uint64_t key, std::string val)
        : right(right), down(down), key(key), val(val) {}
    NODE(): right(nullptr), down(nullptr) {}
};

class MemTable {
  private:
    NODE *head;
    size_t memt_size;
    uint64_t node_num;

  public:
    MemTable();
    ~MemTable();
    size_t size();
    std::string get(const uint64_t &key);
    bool put(const uint64_t &key, const std::string &val);
    bool remove(const uint64_t &key);
    void clear();
    NODE* key_value();
};

// step by step
// 1. implement the bloom filter(learn concept from the internet and inplement it) v
// 2. the relationship between blooom filter and index---how to determine whether a key-value exists v
// 3. implement the two side finding of key K
// 4. never delete, just insert to represent change/delete
// 5. consider how to generate a time stamp: 1 2 3 ... n
// 6. store the data in disk(file) and others in memory, but the file need to contain all the information
// 7. consider how to record the size of memory table
// note: what is stored in the disk is a file; level n has 2^(n+1) sstable; each level's sstable should not be cross except for level 0; a char takes 1 byte in memory