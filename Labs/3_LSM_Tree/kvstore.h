#pragma once

#ifndef KVSTORE_H
#define KVSTORE_H

#include "kvstore_api.h"
#include "MemTable.h"
#include "SSTable.h"

class KVStore : public KVStoreAPI {
    // You can add your implementation here
private:
    MemTable memory_table;

public:
    KVStore(const std::string &dir);

    ~KVStore();

    void put(uint64_t key, const std::string &s) override;

    std::string get(uint64_t key) override;

    bool del(uint64_t key) override;

    void reset() override;

};

#endif  // KVSTORE_H