#include "kvstore.h"

#include <string>

KVStore::KVStore(const std::string &dir) : KVStoreAPI(dir) {
}

KVStore::~KVStore() {}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s) {
    memory_table.put(key, s);
}

/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key) {
    if (memory_table.get(key) != nullptr)
        return *(memory_table.get(key));
    
    // search the sstable
}

/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key) {
    bool del_status = memory_table.remove(key);
    memory_table.put(key, "~DELETED");  // mark a deleted key-value pair
    return false;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset() {}