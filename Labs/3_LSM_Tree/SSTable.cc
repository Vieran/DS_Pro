/* implementation of the SSTable */

#include "SSTable.h"
#include "SSTable_handler.h"
#include <fstream>
#include <iostream>
#include "MurmurHash3.h"
#include "utils.h"

// construct a sstable from a given node list
SSTable::SSTable(std::vector<key_value> &nodes, const uint64_t t_stamp, uint64_t &filenum, const std::string &dirname, const uint32_t level) {
    // set filename
    filepath = dirname + "/level-" +std::to_string(level) + "/" + std::to_string(filenum++) + ".sst";
    // set header
    header.time_stamp = t_stamp;
    header.pair_num = nodes.size();
    header.min_key = (*nodes.begin()).key;
    header.max_key = (*(nodes.end()-1)).key;
    // set index
    uint64_t i = 0;
    uint32_t offset = sizeof(header) + sizeof(bloom_filter) + (sizeof(uint64_t) + sizeof(uint32_t)) * header.pair_num;
    index = new INDEX[header.pair_num];
    for (std::vector<key_value>::iterator it = nodes.begin(); it != nodes.end(); it++) {
        index[i].key = (*it).key;
        index[i].offset = offset;
        i++;
        offset += (*it).val.length();
        set_bf((*it).key);
    }
    // set total size of the data
    total_size = offset;

    // write to file
    // 1. create a dir
    std::string path = "data/level-" + std::to_string(level);
    if (!utils::dirExists(path))
        utils::mkdir((char*)(path.data()));
    // 2. open and write everything except data part
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "write: can not open " << filepath << std::endl;
        exit(-1);
    }
    file.write(reinterpret_cast<char*>(&header), sizeof(header));
    file.write(reinterpret_cast<char*>(bloom_filter), sizeof(bloom_filter));
    for (i = 0; i < header.pair_num; i++)
        //file << index->key << index->offset;
        file.write(reinterpret_cast<char*>(&index[i]), sizeof(uint64_t) + sizeof(uint32_t));
    // 3. write data part
    for (std::vector<key_value>::iterator it = nodes.begin(); it != nodes.end(); it++)
        file.write((char*)((*it).val.data()), sizeof(char) * (*it).val.length());
    file.close();
}

// construct a sstable from a given memtable
// default parameter either on declare or on definition, can not both
SSTable::SSTable(MemTable &memt, uint64_t &t_stamp, uint64_t &filenum, const std::string &dirname, uint64_t level) {
    // set filename
    filepath = dirname + "/level-" + std::to_string(level) + "/" + std::to_string(filenum++) + ".sst";
    // set header
    NODE *key_val = memt.key_value()->right;
    header.time_stamp = t_stamp++;
    header.pair_num = memt.size();
    header.min_key = key_val->key;
    // set index
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
    // 2. open and write everything except for data part
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

// construct a sstable form an exist file
SSTable::SSTable(std::string fp) {
    filepath = fp;
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "read: can not open " << filepath << std::endl;
        exit(-1);
    }

    // set header
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    // set bloomfilter
    file.read(reinterpret_cast<char*>(bloom_filter), sizeof(bloom_filter));
    // set index
    index = new INDEX[header.pair_num];
    uint64_t i;
    for (i = 0; i < header.pair_num; i++)
        file.read(reinterpret_cast<char*>(&index[i]), sizeof(uint64_t) + sizeof(uint32_t));
    // set total size
    file.seekg(0, std::ios::end);
    total_size = file.tellg();
    file.close();
}

SSTable::~SSTable() {
    // delete index
    delete [] index;
}

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
    uint64_t left = 0, right = header.pair_num - 1, middle = 0;
    while (left <= right) {
        middle = left + (right - left) / 2;
        if (index[middle].key == key) {
            return true;
        }
        if (index[middle].key > key) {
            if (middle <= 0)
                break;
            right = middle - 1;
        } else
            left = middle + 1;
    }
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
        middle = left + (right - left) / 2;
        if (index[middle].key == key) {
            offset = index[middle].offset;
            flag = true;
            break;
        }
        if (index[middle].key > key) {
            if (middle <= 0)
                break;
            right = middle - 1;
        } else
            left = middle + 1;
    }
    if (!flag)  // key-value not found
        return "";
    
    // open file and read the value
    std::string value;
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

// retrun the time stamp of sstable
uint64_t SSTable::get_timestamp() {
    return header.time_stamp;
}

// return the min key of sstable
uint64_t SSTable::get_minkey() {
    return header.min_key;
}

// return the max key of sstable
uint64_t SSTable::get_maxkey() {
    return header.max_key;
}

// return the pair num of keys of sstable
uint64_t SSTable::get_pairnum() {
    return header.pair_num;
}

// return the i^th key in the sstable
uint64_t SSTable::get_index_key(uint64_t i) {
    return index[i].key;
}

// delete the file
void SSTable::rmfile() {
    utils::rmfile((char*)(filepath.data()));
}