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
    node_t(node_t *prev, unsigned hv, unsigned long pos):prev(prev), next(nullptr), parent(nullptr), hash_val(hv), position(pos) {}
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

    // re-compute the hash tree
    void build_tree(level_list *&ls);
    // insert a hash value to the end of the level list
    void insert(level_list *&ls, unsigned long hash_val);
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
    for (std::vector<unsigned long>::iterator tmp = leaf_nodes.begin(); tmp != last; tmp++)
        insert(node_list, *tmp);
    node_list->up_level = nullptr;
}

Merkle_Tree::~Merkle_Tree() {}

// re-compute the hash tree after a new leaf node insert
void Merkle_Tree::build_tree(level_list *&ls) {
    // get the new insert node
    node_t *new_node = ls->last_node;

    unsigned long p_hash;
    level_list *current_level = ls;
    if (ls == node_list && new_node->position == 1) {  // insert the first node
        current_level->up_level = new level_list;
        current_level = current_level->up_level;
        root = current_level->first_node = new node_t(nullptr, new_node->hash_val, 1);
        current_level->last_node = current_level->first_node;
        new_node->parent = root;  // link
    } else if (ls == node_list && new_node->position == 2) {  // insert the second node
        root->hash_val = new_node->hash_val + new_node->prev->hash_val;
        new_node->parent = root;
    } else if (new_node->position % 2 == 0) {  // even postition
        node_t *current_node = new_node;
        // update all the current_nodes' hash value
        while (current_node && current_node->prev && current_node->position != 2 ) {
            if (current_node->position % 2 == 0) {
                p_hash = current_node->hash_val + current_node->prev->hash_val;
                if (!current_node->parent)
                    current_node->parent = current_node->prev->parent;  // link
            } else {
                p_hash = current_node->hash_val;
            }
            current_node = current_node->parent;  // change current node to parent node
            current_level = current_level->up_level;  // move to upper level
            current_node->hash_val = p_hash;
        }

        if (current_node && current_node->position == 2 && !current_node->prev->parent) {  // update root
            p_hash = current_node->hash_val + current_node->prev->hash_val;
            current_level->up_level = new level_list;  // create a new level
            current_level = current_level->up_level;
            root = current_level->first_node = new node_t(nullptr, p_hash, 1);
            current_level->last_node = current_level->first_node;
            current_node->parent = current_node->prev->parent = root;  // link
        } else {
            p_hash = current_node->hash_val + current_node->prev->hash_val;
            current_node->parent = current_node->prev->parent;  // link
            current_node->parent->hash_val = p_hash;
        }
    } else {  // odd position, need to add new parent
        p_hash = new_node->hash_val;
        if (!new_node->parent && current_level->up_level) {
            insert(current_level->up_level, p_hash);
            new_node->parent = current_level->up_level->last_node;  // link
        } else
            new_node->parent->hash_val = p_hash;
    }
}

// insert a hash value to the end of the level list
void Merkle_Tree::insert(level_list *&ls, unsigned long hash_val) {
    if (!ls->first_node) {  // first leaf node
        ls->first_node = new node_t(nullptr, hash_val, 1);
        ls->last_node = ls->first_node;
    } else {
        ls->last_node->next = new node_t(ls->last_node, hash_val, ls->last_node->position+1);
        ls->last_node = ls->last_node->next;
    }

    build_tree(ls);
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
        if (node->next && node->position %2 != 0)  // odd position
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
    insert(node_list, hash_value);
}

//  return the root hash value of the Merkle Tree
unsigned long Merkle_Tree::getRootHash() {
    if (!root)
        return 0;
    return root->hash_val;
}

/*
int main() {
    std::vector<unsigned long> leaf_node;
    for(unsigned long i = 0; i < 10; i++)
        leaf_node.push_back(i);
    Merkle_Tree mt(leaf_node);
    mt.getRootHash();
    std::vector<unsigned long> getp = mt.getProof(0);
    for (std::vector<unsigned long>::iterator it = getp.begin(); it != getp.end(); it++)
        std::cout << *it << " ";
    std::cout << std::endl;
    return 0;
}
*/