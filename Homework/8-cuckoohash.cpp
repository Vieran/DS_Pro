/*
a. 分别分析以下三种哈希表实现方法的插入、查找时间复杂度，并
b. 选择这三个实现方法中的一个实现以下C++哈希表类
请提供源码、运行截图与文档。


注意：我们只插入正数val，所以在查找失败时返回-1
注意2:请不要使用C++ STL中已有的哈希表实现，仅能使用std::vector, std::list, std::pair
注意3:不要修改下面的main函数，用它进行运行测试，与"应当输出"做对比，检查实现是否正确。提供运行截图

1.分离链表法 (separate chaining)
2.线性探测法 (linear probing)
3.布谷哈希法 (cuckoo hashing)

*/

#include <iostream>
#include <map>
#define maxlen 100
#define halflen 50
using namespace std;

class HashTable {  // your implementation
private:
    pair<int, int> hashtable[maxlen];
    int hash1(const int &key);
    int hash2(const int &key);
    int get1(const int &key);
    int get2(const int &key);
    void replace(int key, int val);
    void swap_element(pair<int, int> *a, pair<int, int> *b);

public:
    HashTable();
    void put(int key, int val);
    int get(int key);  // return -1 on searching failure
    void remove(int key);
};

HashTable::HashTable() {
    for (int i = 0; i < maxlen; i++)
        hashtable[i].first = -1;
}
int HashTable::hash1(const int &key) {
    return key % halflen;
}
int HashTable::hash2(const int &key) {
    return key / halflen % halflen + halflen;
}
int HashTable::get1(const int &key) {
    pair<int, int> element = hashtable[hash1(key)];
    return (element.first == key) ? element.second : -1;
}
int HashTable::get2(const int &key) {
    pair<int, int> element = hashtable[hash2(key)];
    return (element.first == key) ? element.second : -1;
}
void HashTable::replace(int key, int val) {
    if (hashtable[hash1(key)].first == key)
        hashtable[hash1(key)].second = val;
    else
        hashtable[hash2(key)].second = val;
}
void HashTable::swap_element(pair<int, int> *a, pair<int, int> *b) {
    pair<int, int> tmp = *a;
    *a = *b;
    *b = tmp;
}

void HashTable::put(int key, int val) {
    if (key == -1) {
        printf("invalid key\n");
        return;
    }
    if (get(key) != -1) {
        replace(key, val);
        return;
    }

    if (hashtable[hash1(key)].first == -1) {
        hashtable[hash1(key)].first = key;
        hashtable[hash1(key)].second = val;
    } else if (hashtable[hash2(key)].first == -1) {
        hashtable[hash2(key)].first = key;
        hashtable[hash2(key)].second = val;
    } else {
        pair<int, int> evicted = pair<int, int>(key, val);
        int which = 0;
        int idx = hash1(evicted.first);
        int pre_pos = -1;
        while (hashtable[idx].first != -1){
            printf("evicted pair (%d, %d) from %d to %d\n", evicted.first, evicted.second, pre_pos, idx);
            swap_element(&evicted, &hashtable[idx]);
            pre_pos = idx;
            which = 1 - which;
            idx = (which == 0) ? hash1(evicted.first) : hash2(evicted.first);
        }
        printf("evicted pair (%d, %d) from %d to %d\n", evicted.first, evicted.second, pre_pos, idx);
        hashtable[idx] = evicted;
    }
}
int HashTable::get(int key) {
    if (key == -1) {
        printf("invalid key\n");
        return -1;
    }

    int val = get1(key);
    if (val == -1)
        return get2(key);
    return val;
}
void HashTable::remove(int key) {
    if (key == -1) {
        printf("invalid key\n");
        return;
    }

    if (hashtable[hash1(key)].first == key)
        hashtable[hash1(key)].first = -1;
    else if (hashtable[hash2(key)].first == key)
        hashtable[hash2(key)].first = -1;
    /*
    else
        printf("no such key\n");
    */
}

int main() {
    HashTable ht = HashTable();
    ht.put(1, 1);
    ht.put(2, 2);
    cout << ht.get(1) << ' ';
    cout << ht.get(3) << ' ';
    ht.put(2, 1);
    cout << ht.get(2) << ' ';
    ht.remove(2);
    cout << ht.get(2) << ' ';
    ht.remove(3);
    cout << ht.get(3) << ' ';
}  // 应当输出1 -1 1 -1 -1