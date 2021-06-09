/* implementation of the MemTable*/
#include "MemTable.h"

#include <vector>

MemTable::MemTable() {
    head = new NODE();
    memt_size = 0;
    node_num = 0;
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

// clear all key value
void MemTable::clear() {
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

    head = new NODE();
    memt_size = 0;
    node_num = 0;
}

// return the num of nodes in the memtable
size_t MemTable::size() { return node_num; }

std::string MemTable::get(const uint64_t &key) {
    if (node_num == 0)
        return "";
    NODE *p = head;
    while (p) {
        while (p->right && p->right->key <= key) {
            if (p->right->key == key)
                return p->right->val;
            p = p->right;
        }
        p = p->down;
    }
    return "";
}

bool MemTable::put(const uint64_t &key, const std::string &val) {
    size_t node_size = sizeof(uint64_t) + sizeof(uint32_t) + val.length();  // for char, the length is the byte it takes; offset is uint32_t
    if (memt_size + node_size > MAXSIZE)  // need to transfer it into a sstable
        return false;

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
        size_t tmp_size = memt_size - (p->val).length() + val.length();
        if (tmp_size > MAXSIZE)
            return false;
        while (p) {
            p->val = val;
            p = p->down;
        }
        memt_size = tmp_size;
        return true;
    }

    bool insertUp = true;
    NODE *downNode = nullptr;
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
    node_num++;

    return true;
}

bool MemTable::remove(const uint64_t &key) {
    if (node_num == 0)
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
    size_t node_size = sizeof(uint64_t) + sizeof(uint32_t) + pathlist.back()->val.length();
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
    node_num--;
    return true;
}

// return all the key-value list
NODE* MemTable::key_value() {
    NODE *tmp = head;
    // find the bottom level, which contains all the elements in the list
    while (tmp->down != nullptr)
        tmp = tmp->down;
    return tmp;
}

// if the memory table is empty
bool MemTable::empty() {
    return node_num == 0 ? true : false;
}