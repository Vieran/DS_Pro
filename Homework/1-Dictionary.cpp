/* description: https://202.120.40.8:30463/contest/2/problem/1
 * start: 2021-2-27
 * timelimit: 4days
 */

#include <iostream>
#include <algorithm>

template <typename K, typename V>
class Dictionary {
    public:
    struct dic {
        K key;
        V value;
        dic *next;
    };

    dic *head; // remember that head is a blank dic

    /* create a new dic */
    Dictionary() {
        head = new dic;
        head->next = NULL;
    }

    /* get the length of the dic */
    int size() const {
        int i = 0;
        dic *tmp = head;
        while (tmp->next) {
            tmp = tmp->next;
            i++;
        }
        return i;
    }

    /* insert a new value */
    bool put(K k, V v) {
        // traverse the list
        dic *tmp = head->next;
        while(tmp) {
            if(tmp->key == k) {
                tmp->value = v;
                return true;
            }
            tmp = tmp->next;
        }

        // create a new dic
        dic *new_dic = new dic;
        new_dic->key = k;
        new_dic->value = v;
        new_dic->next = NULL;

        // link the new dic to the list
        tmp = head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_dic;
        return true;
    }

    /* get the value of given key */
    V *get(K k) const{
        dic *tmp = head->next;
        while (tmp) {
            if (tmp->key == k)
                return &(tmp->value);
            else
                tmp = tmp->next;
        }
        return nullptr;
    }

    /* delete a dic from the list*/
    bool remove(K k) {
        dic *pre = NULL;
        dic *cur = head;
        while (cur) {
            if (cur->key == k) {
                pre->next = cur->next;
                delete cur;
                return true;
            } else {
                pre = cur;
                cur = cur->next;
            }
        }
        return false;
    }

    /* destroy the dic */
    ~Dictionary() {
        dic *tmp = head->next;
        dic *q = tmp;
        while(q) {
            tmp = q;
            q = tmp->next;
            delete tmp;
        }
        delete head;
    }

    template <typename K1, typename V1>
    friend std::ostream &operator<<(std::ostream &os, const Dictionary<K1, V1> &dic);
};

/* it seems that this func will not be detected correctly when using template */
/*
template <typename K1, typename V1>
int compare(const void *a, const void *b) {
    return 0;
}
*/

template <typename K1, typename V1>
std::ostream &operator<<(std::ostream &os, const Dictionary<K1, V1> &dic) {
    int len = dic.size();
    K1 *arr = new K1[len];
    typename Dictionary<K1, V1>::dic *tmp = dic.head->next;
    for(int i = 0; i < len; i++) {
        arr[i] = tmp->key;
        tmp = tmp->next;
    }
    std::sort(arr, arr+len);
    V1 value;
    for(int i = 0; i < len; i++) {
        value = *(dic.get(arr[i]));
        os << arr[i] << " " << value << " " ;
    }
    delete arr;
    return os;
}

int main() {
    Dictionary<int,double> dict;
    dict.put(1,2.1);
    dict.put(5,3.1);
    dict.put(3,4.1);
    dict.remove(2);
    dict.put(4,5.0);
    std::cout << dict << std::endl;
    dict.put(4,5.4);
    std::cout << dict << std::endl;
    return 0;
}