/* definition of the MemTable */
#pragma once

#include <cstdint>
#include <string>

struct NODE {
    NODE *right, *down;
    int64_t key;
    std::string val;
    NODE(NODE *right, NODE *down, int64_t key, std::string val)
        : right(right), down(down), key(key), val(val) {}
    NODE(): right(nullptr), down(nullptr) {}
};

class MemTable {
   private:
    NODE *head;
    size_t memt_size;

   public:
    MemTable();
    ~MemTable();
    size_t size();
    std::string* get(const int64_t &key);
    void put(const int64_t &key, const std::string &val);
    bool remove(const int64_t &key);
};
