#pragma once
unsigned long hash64shift(unsigned long key)
{
  key = (~key) + (key << 21); // key = (key << 21) - key - 1;
  key = key ^ (key >> 24);
  key = (key + (key << 3)) + (key << 8); // key * 265
  key = key ^ (key >> 14);
  key = (key + (key << 2)) + (key << 4); // key * 21
  key = key ^ (key >> 28);
  key = key + (key << 31);
  return key;
}

vector<unsigned long> Merkle_Tree::getProof(unsigned long hash_value);
Merkle_Tree::Merkle_Tree();
Merkle_Tree::Merkle_Tree(vector<unsigned long>);
void Merkle_Tree::addTransaction(unsigned long);
unsigned long Merkle_Tree::getRootHash();