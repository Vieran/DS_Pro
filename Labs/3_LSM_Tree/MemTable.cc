/* implementation of the MemTable*/
#include "MemTable.h"

#include <vector>

MemTable::MemTable() {
    head = new NODE();
    memt_size = 0;
}

MemTable::~MemTable() {
    NODE *current_level = head;
    NODE *next_level;
    while (current_level) {
        next_level = current_level->down;
        NODE *tmp;
        while (current_level->right != nullptr) {
            tmp = current_level->right;
            current_level->right = tmp->right;
            free(tmp);
        }
        free(current_level);
        current_level = next_level;
    }
}

// return the num of nodes in the memtable
size_t MemTable::size() { return memt_size; }

std::string* MemTable::get(const int64_t &key) {
    if (memt_size == 0)
        return nullptr;
    NODE *p = head;
    while (p) {
        while (p->right && p->right->key <= key) {
            if (p->right->key == key)
                return &(p->right->val);
            p = p->right;
        }
        p = p->down;
    }
    return nullptr;
}

void MemTable::put(const int64_t &key, const std::string &val) {
    size_t node_size = sizeof(int64_t) + val.capacity();
    /* TODO: whether to put into sstable */

    std::vector<NODE *> pathlist;  // record the search path
    NODE *p = head;
    bool exist = false;
    while (p) {
        while (p->right && p->right->key < key)
            p = p->right;
        if (p->right && p->right->key == key) {
            p = p->right;
            exist = true;
            break;
        }
        pathlist.push_back(p);
        p = p->down;
    }

    // update key-value pair
    if (exist) {
        memt_size -= sizeof(p->val);
        while (p) {
            p->val = val;
            p = p->down;
        }
        memt_size += sizeof(val);
        return;
    }

    bool insertUp = true;
    NODE *downNode = nullptr;
    size_t new_node_size = 0;
    while (insertUp && pathlist.size() > 0) {
        NODE *insert = pathlist.back();
        pathlist.pop_back();
        insert->right = new NODE(insert->right, downNode, key, val);  // link new node
        downNode = insert->right;  // take new node as the next downnode
        insertUp = (rand() & 1);
    }
    if (insertUp) {
        NODE *oldHead = head;
        head = new NODE();
        head->right = new NODE(nullptr, downNode, key, val);
        head->down = oldHead;
    }
    memt_size += node_size;
}

bool MemTable::remove(const int64_t &key) {
    if (memt_size == 0)
        return false;
    
    std::vector<NODE *> pathlist;  // record the search path
    NODE *p = head;
    while (p) {
        while (p->right && p->right->key < key)
            p = p->right;
        if (p->right && p->right->key == key)
            pathlist.push_back(p);
        p = p->down;
    }

    if (pathlist.empty())
        return false;

    // delete and link
    NODE *tmp;
    NODE *preNode;
    size_t node_size = sizeof(int64_t) + pathlist.back()->val.capacity();
    while (!pathlist.empty()) {
        preNode = pathlist.back();
        if (preNode == head && head->right->right == nullptr)  // delete at top level
            head = head->down;
        tmp = preNode->right;
        preNode->right = tmp->right;
        free(tmp);
        pathlist.pop_back();
    }
    memt_size -= node_size;
    return true;
}