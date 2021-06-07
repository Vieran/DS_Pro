/* implement of SSTable_handler */

#include "SSTable_handler.h"
#include "SSTable.h"
#include "utils.h"

SSTable_handler::SSTable_handler() {
    t_stamp = 0;
    std::vector<SSTable*> first_ele;
    sst_handler_list.push_back(first_ele);
}

SSTable_handler::~SSTable_handler() {
    for (std::vector<std::vector<SSTable*>>::iterator it = sst_handler_list.begin(); it != sst_handler_list.end(); it++) {
        for (std::vector<SSTable*>::iterator sit = (*it).begin(); sit != (*it).end(); sit++)
            delete *sit;
    }
}

// judge if a key-value pair exist in the sstable
// this just use the bloom filter, so false means not exist, but true does not means exist
bool SSTable_handler::exist(uint64_t key) {
    for (std::vector<std::vector<SSTable*>>::iterator it = sst_handler_list.begin(); it != sst_handler_list.end(); it++) {
        for (std::vector<SSTable*>::iterator sit = (*it).begin(); sit != (*it).end(); sit++) {
            if ((**sit).exist(key))  // search in this sstable
                return true;
        }
    }
    return false;
}

// get the value of the given key from the sstable
std::string SSTable_handler::get(uint64_t key) {
    std::vector<SSTable*> tmp;  // store the sstable, where the key exist
    for (std::vector<std::vector<SSTable*>>::iterator it = sst_handler_list.begin(); it != sst_handler_list.end(); it++) {
        for (std::vector<SSTable*>::iterator sit = (*it).begin(); sit != (*it).end(); sit++) {
            if ((**sit).exist(key))  // search in this sstable
                tmp.push_back(*sit);
        }
    }
    if (tmp.empty())
        return "";
    
    // find the newest sstable
    SSTable* newest = *(tmp.begin());
    for (std::vector<SSTable*>::iterator i = tmp.begin(); i != tmp.end(); i++)
        if ((*i)->get_timestamp() > newest->get_timestamp())
            newest = (*i);
    return newest->get(key);
}

void SSTable_handler::compaction() {
}

void SSTable_handler::to_sst(MemTable &memt) {
    // genarate a sstable in memory
    SSTable *tmp = new SSTable(memt, t_stamp);
    sst_handler_list[0].push_back(tmp);

    // todo: check and apply compaction
}

void SSTable_handler::read_file(const std::string &dir) {
    dir_name = dir;
}

// delete all file and clear the sst_handler_list
void SSTable_handler::clear() {
    sst_handler_list.clear();
    t_stamp = 0;

    if (utils::dirExists(dir_name)) {
        std::vector<std::string> files;
        if (utils::scanDir(dir_name, files) != 0)
            for (std::vector<std::string>::iterator f = files.begin(); f != files.end(); f++)
                utils::rmfile((char*)((*f).data()));
        utils::rmdir((char*)(dir_name.data()));
    }

    std::vector<SSTable*> first_ele;
    sst_handler_list.push_back(first_ele);
}