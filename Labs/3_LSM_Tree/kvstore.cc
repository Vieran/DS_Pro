#include "kvstore.h"
#include <string>

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir) {
    // read from the dir
    sst_h.read_file(dir);
}

KVStore::~KVStore() {
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s) {
    if (!memt.put(key, s)) {
        sst_h.to_sst(memt);  // change current memory table into a sstable
        memt.clear();  // clear current memory table
        memt.put(key, s);  // insert the key-value pair
    }
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key) {
    std::string s = memt.get(key);
    if (s == "")  // not in memory table
        s = sst_h.get(key);  // search the sstable

    if (s == "~DELETED~")
        return "";
    else
        return s;
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key) {
    std::string tmp1 = memt.get(key);
    if (tmp1 == "~DELETED~")  // deleted
        return false;

    std::string tmp2 = sst_h.get(key);
    if (tmp1 != "" || (tmp2 != "" && tmp2 != "~DELETED~")) {  // exist(which means not a "deleted" mark)
        memt.remove(key);  // remove
        std::string tmp = "~DELETED~";
        put(key, tmp);
        return true;  // return ture
    }
    
    memt.remove(key);  // remove
    std::string tmp = "~DELETED~";
    put(key, tmp);
    return false;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset() {
    memt.clear();
    sst_h.clear();
}