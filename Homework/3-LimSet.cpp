/* description: https://202.120.40.8:30463/contest/4/problem/2
 * start: 2021-3-13
 * timelimit: 4days
 */

#include <iostream>
#include <map>

template<typename K>
class LimSet {
private:
    int size, cap;
    std::map<K, long long int> limset;
    long long int priority;

public:
    explicit LimSet(int capacity) { // explicit forbid implicit conversion
        size = 0;
        cap = capacity;
        priority = 0;
    }

    bool in(const K &key) {
        if(limset.find(key) != limset.end()) {
            priority++;
            limset[key] = priority;
            return true;
        } else
            return false;
    }

    void add(const K &key) {
        if(limset.size() == cap) {
            K key_to_delete;
            long long int min = (1LL << 32);
            typename std::map<K, long long int>::iterator tmp = limset.begin();
            while(tmp != limset.end()) {
                if(tmp->second < min) {
                    min = tmp->second;
                    key_to_delete = tmp->first;
                }
                tmp++;
            }
            limset.erase(key_to_delete);
        }
        priority++;
        limset[key] = priority;
    }

    void remove(const K &key) {
        limset.erase(key);
    }
};

int main() {
    LimSet<int> limSet(2);
    limSet.add(1);
    limSet.add(2);
    std::cout << limSet.in(2) << std::endl;
    std::cout << limSet.in(1) << std::endl;
    limSet.add(3);
    std::cout << limSet.in(2) << std::endl;
    return 0;
}

/* Reference
 * explicit key word: https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean
 * bloom filter(wikipedia): https://zh.wikipedia.org/wiki/%E5%B8%83%E9%9A%86%E8%BF%87%E6%BB%A4%E5%99%A8
 * bloom filter concept and theory(csdn): https://blog.csdn.net/jiaomeng/article/details/1495500
 * bloom filter by example: https://llimllib.github.io/bloomfilter-tutorial/
 */
