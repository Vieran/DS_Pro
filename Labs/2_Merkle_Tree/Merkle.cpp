#include <vector>
//#include <iostream>

#pragma once
unsigned long hash64shift(unsigned long key) {
    key = (~key) + (key << 21);  // key = (key << 21) - key - 1;
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8);  // key * 265
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4);  // key * 21
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return key;
}

// construct and insert new leaf node in order. More specific, the order of the
// hash values in the leaf level should be the same as the transaction list and
// insertion order hint: use a linked list to link all leaf node to support
// insertion to last level, and maintain a parent pointer to re-compute the
// hashes use "+" to get upper root hash
struct node_t {
    node_t *prev;
    node_t *next;
    node_t *parent;
    unsigned long hash_val;  // the hash value of this node
    unsigned long position;  // the position of the node in this list
    node_t(node_t *prev, unsigned long hv, unsigned long pos):prev(prev), next(nullptr), parent(nullptr), hash_val(hv), position(pos) {}
};
struct level_list {
    level_list *up_level;
    node_t *first_node;  // ptr to the first node in this level
    node_t *last_node;  // ptr to the last node int this level
    level_list():up_level(nullptr), first_node(nullptr), last_node(nullptr) {}
};
class Merkle_Tree {
private:
    node_t *root;           // Merkle root
    level_list *node_list;  // list to link leaf nodes

    // construct at one level
    void build_tree(level_list *&ls);
    // insert a hash value to the end of the level list
    void insert_tool_func(level_list *&ls, unsigned long hash_val);
    // insert a hash value
    void insert(unsigned long hash_val);
    // find the hash value in the leaf nodes
    node_t* find_node(unsigned long hash_value);

public:
    Merkle_Tree();
    ~Merkle_Tree();
    Merkle_Tree(std::vector<unsigned long>);
    std::vector<unsigned long> getProof(unsigned long hash_value);
    void addTransaction(unsigned long);
    unsigned long getRootHash();
};

// construct an empty merkle tree
Merkle_Tree::Merkle_Tree() {
    node_list = new level_list;
    root = nullptr;
}

// construct a merkle tree from all given leaf nodes(hash values)
Merkle_Tree::Merkle_Tree(std::vector<unsigned long> leaf_nodes) {
    std::vector<unsigned long>::iterator last = leaf_nodes.end(); // point to the next of the last element(nullptr in fact)
    node_list = new level_list;
    root = nullptr;
    for (std::vector<unsigned long>::iterator tmp = leaf_nodes.begin(); tmp != last; tmp++)
        insert(*tmp);
}

Merkle_Tree::~Merkle_Tree() {
    level_list *tmp = node_list;
    while (tmp) {
        node_t *node = tmp->first_node;
        while (node) {
            node = node_list->first_node;
            node_list->first_node = node->next;
            delete node;
            node = node_list->first_node;
        }
        node_list = node_list->up_level;
        delete tmp;
        tmp = node_list;
    }
}

// construct at one level
void Merkle_Tree::build_tree(level_list *&ls) {
    if (!ls)  // this level is not create yet
        return;
    
    node_t *current_node = ls->last_node;
    level_list *current_level = ls;
    unsigned long p_hash;
    while (current_node != root) {
        if (current_node->position % 2 == 0) {  // node at even position
            node_t *pre = current_node->prev;
            p_hash = hash64shift(current_node->hash_val + pre->hash_val);
            if (pre->parent) {  // parent exits
                current_node->parent = pre->parent;  // link
                current_node = current_node->parent;
                current_node->hash_val = p_hash;  // update parent hash value
                current_level = current_level->up_level;  // go to upper level
            } else {  // parent not exits, which means its position is 2
                // new root
                current_level->up_level = new level_list;
                insert_tool_func(current_level->up_level, p_hash);
                pre->parent = current_node->parent = current_level->up_level->last_node;  // link
                root = current_node->parent;  // update root
                return;
            }
        } else if (current_node->position % 2 != 0) {  // node at odd position
            p_hash = current_node->hash_val;
            if (current_node->parent) {  // parent exist
                current_node->parent->hash_val = p_hash;  // update parent hash value
            } else {  // parent not exist, need to new a parent
                insert_tool_func(current_level->up_level, p_hash);
                current_node->parent = current_level->up_level->last_node;  // link
            }
            current_node = current_node->parent;
            current_level = current_level->up_level;  // go to upper level
        }
    }
}

// insert a hash value to the end of the level list
void Merkle_Tree::insert_tool_func(level_list *&ls, unsigned long hash_val) {
    if (!ls->first_node) {  // first node in this level
        ls->first_node = new node_t(nullptr, hash_val, 1);
        ls->last_node = ls->first_node;
    } else {
        ls->last_node->next = new node_t(ls->last_node, hash_val, ls->last_node->position+1);
        ls->last_node = ls->last_node->next;
    }
}

// insert a hash value
void Merkle_Tree::insert(unsigned long hash_val) {
    // first insertion, need to initialize the root
    if (!node_list->first_node) {
        insert_tool_func(node_list, hash_val);
        level_list *up = new  level_list;
        insert_tool_func(up, hash_val);
        node_list->up_level = up;
        root = up->first_node;
        node_list->first_node->parent = root;
        return;
    }
    insert_tool_func(node_list, hash_val);
    build_tree(node_list);
}

// find the hash value in the leaf nodes
node_t* Merkle_Tree::find_node(unsigned long hash_value) {
    node_t *tmp = node_list->first_node;
    while(tmp) {
        if (tmp->hash_val == hash_value)
            return tmp;
        tmp = tmp->next;
    }
    return nullptr;
}

//  given a hash value, return an ordered hash value tree path
std::vector<unsigned long> Merkle_Tree::getProof(unsigned long hash_value) {
    std::vector<unsigned long> list_re;
    node_t *node = find_node(hash_value);

    // reverse the Merkle tree
    while (node && node != root) {
        if (node->position % 2 == 0)  // even position
            if (node->prev)
                list_re.push_back(node->prev->hash_val);
            else
                list_re.push_back(0);
        if (node->position %2 != 0)  // odd position
            if (node->next)
                list_re.push_back(node->next->hash_val);
            else
                list_re.push_back(0);
        node = node->parent;
    }
    return list_re;
}

// insert a new leaf node to the Merkle Tree, need to re-compute the hash values of the tree
void Merkle_Tree::addTransaction(unsigned long hash_value) {
    insert(hash_value);
}

//  return the root hash value of the Merkle Tree
unsigned long Merkle_Tree::getRootHash() {
    if (!root)
        return 0;
    return root->hash_val;
}

/*
int main(int argc, char **argv) {
    std::vector<unsigned long> leaf_node;
    Merkle_Tree mt;
    for(unsigned long i = 0; i < 6; i++)
        mt.addTransaction(i);
    //for (std::vector<unsigned long>::iterator it = leaf_node.begin(); it != leaf_node.end(); it++) {
    for (unsigned long i = 0; i < 6; ++i) {
        std::vector<unsigned long> getp = mt.getProof(i);
        for (std::vector<unsigned long>::iterator it_ = getp.begin(); it_ != getp.end(); it_++)
            std::cout << *it_ << " ";
        std::cout << std::endl;
    }
    std::cout << mt.getRootHash() << std::endl;
    return 0;
}
*/