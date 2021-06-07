/* implementation of the SSTable */

#include "SSTable.h"
#include <fstream>
#include <iostream>
#include "MurmurHash3.h"
#include "utils.h"

SSTable::SSTable() {}

// construct a sstable from a given memtable
// default parameter either on declare or on definition, can not both
SSTable::SSTable(MemTable &memt, uint64_t &t_stamp, uint64_t level) {
    filename = std::to_string(t_stamp) + ".sst";
    NODE *key_val = memt.key_value()->right;
    header.time_stamp = t_stamp++;
    header.pair_num = memt.size();
    header.min_key = key_val->key;
    NODE *tmp = key_val;
    uint64_t i = 0;
    uint32_t offset = sizeof(header) + sizeof(bloom_filter) + (sizeof(uint64_t) + sizeof(uint32_t)) * header.pair_num;
    index = new INDEX[header.pair_num];
    while (tmp != nullptr) {  // find the max key and construct index and bloom filter
        header.max_key = tmp->key;
        index[i].key = tmp->key;
        index[i].offset = offset;
        i++;
        offset += tmp->val.length();
        set_bf(tmp->key);
        tmp = tmp->right;
    }
    total_size = offset;

    /* write into file */
    // 1. create a dir
    std::string path = "data/level-" + std::to_string(level);
    if(!utils::dirExists(path))
        utils::mkdir((char*)(path.data()));
    // 2. open and write the everything except for data part
    std::string filepath = path + "/" + filename;
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "write: can not open " << filepath << std::endl;
        exit(-1);
    }
    file.write(reinterpret_cast<char*>(&header), sizeof(header));
    //file << header.time_stamp << header.pair_num << header.min_key << header.max_key;
    file.write(reinterpret_cast<char*>(bloom_filter), sizeof(bloom_filter));
    for (i = 0; i < header.pair_num; i++)
        //file << index->key << index->offset;
        file.write(reinterpret_cast<char*>(&index[i]), sizeof(uint64_t) + sizeof(uint32_t));
    // 3. write data part
    tmp = key_val;
    while (tmp != nullptr) {
        //file << key_val->val;
        file.write((char*)((tmp->val).data()), sizeof(char) * tmp->val.length());
        tmp = tmp->right;
    }
    file.close();
}

SSTable::~SSTable() {}

// setup a bloom filter for a new added key
void SSTable::set_bf(uint64_t key) {
    unsigned int hash[4] = {0};
    MurmurHash3_x64_128(&key, sizeof(key), 1, hash);

    bloom_filter[hash[0] % 10240] = true;
    bloom_filter[hash[1] % 10240] = true;
    bloom_filter[hash[2] % 10240] = true;
    bloom_filter[hash[3] % 10240] = true;
}

// use bloom filter to judge if the node exist
// false: cannot exist; true: probably exist
bool SSTable::exist(uint64_t key) {
    unsigned int hash[4] = {0};
    MurmurHash3_x64_128(&key, sizeof(key), 1, hash);

    if (!bloom_filter[hash[0] % 10240] || !bloom_filter[hash[1] % 10240] || !bloom_filter[hash[2] % 10240] || !bloom_filter[hash[3] % 10240])
        return false;
    
    // search if the key really exist
    for (uint64_t i = 0; i < header.pair_num; i++)
        if (index[i].key == key)
            return true;
    return false;
}

// use binary search to quickly find the key
// fetch the value from the file in the disk
std::string SSTable::get(uint64_t key) {
    // binary search
    uint64_t left = 0, right = header.pair_num - 1, middle = 0;
    uint32_t offset;
    bool flag = false;
    while (left <= right) {
        middle = left + ((right - left) >> 1);
        if (index[middle].key > key)
            right = middle - 1;
        else if (index[middle].key < key)
            left = middle + 1;
        else {
            offset = index[middle].offset;
            flag = true;
            break;
        }
    }
    if (!flag)  // key-value not found
        return "";
    
    // open file and read the value
    std::string value;
    std::string filepath = "data/level-0/" + filename;
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "read: can not open " << filepath << std::endl;
        exit(-1);
    }
    file.seekg(offset);
    uint32_t num_char;
    if (middle == header.pair_num - 1)  // at the end of the file
        num_char = total_size - offset;
    else  // read the length of the string
        num_char = index[middle + 1].offset - offset;
    
    char tmp;
    for (uint32_t i = 0; i < num_char; i++) {
        file.read(&tmp, sizeof(tmp));
        value += tmp;
    }
    file.close();
    
    return value;
}

// retrun the time stamp of given sstable
uint64_t SSTable::get_timestamp() {
    return header.time_stamp;
}