/* description: https://202.120.40.8:30463/contest/3/problem/1
 * start: 2021-3-6
 * timelimit: 5days
 */

#include <iostream>
#include <vector>
using namespace std;

template<typename K, typename V>
struct Node{
    Node<K, V> *right,*down;   //向右向下足矣
    K key;
    V val;
    Node(Node<K, V> *right,Node<K, V> *down, K key, V val): right(right), down(down), key(key), val(val){}
    Node(): right(nullptr), down(nullptr) {}
};

template<typename K, typename V>
class Skiplist {
public:
    Node<K, V> *head;
    size_t num_of_nodes;
    Skiplist() {
        head = new Node<K, V>();  //初始化头结点
        num_of_nodes = 0; // initialize the num of nodes
    }

    size_t size(){
        return num_of_nodes;
    }
    
    V* get(const K& key) {
        if (num_of_nodes == 0)
            return nullptr;
        Node<K,V> *p = head;
        while (p) {
            while(p->right && p->right->key <= key) {
                if (p->right->key == key)
                    return &(p->right->val);
                p = p->right;
            }
            p = p->down;
        }
        return nullptr;
    }
    
    void put(const K& key, const V& val) {
        vector<Node<K, V>*> pathList;    //从上至下记录搜索路径
        Node<K, V> *p = head;
        while(p){
            while(p->right && p->right->key < key){ 
                p = p->right;
            }
            pathList.push_back(p);
            p = p->down;
        }

        bool insertUp = true;
        Node<K, V>* downNode= nullptr; // first insert at the bottom, so it should be nullptr
        while(insertUp && pathList.size() > 0){   //从下至上搜索路径回溯，50%概率
            Node<K, V> *insert = pathList.back();
            pathList.pop_back();
            insert->right = new Node<K, V>(insert->right, downNode, key, val); //add新结点
            downNode = insert->right;    //把新结点赋值为downNode
            insertUp = (rand()&1);   //50%概率
        }
        if(insertUp){  //插入新的头结点，加层
            Node<K, V> * oldHead = head;
            head = new Node<K, V>();
            head->right = new Node<K, V>(NULL, downNode, key, val);
            head->down = oldHead; // it is different from the text book. there is no sentry.
        }
        num_of_nodes++;
    }
    
    bool remove(const K& key) {
        if (num_of_nodes == 0)
            return false;
        Node<K,V> *p = head;
        Node<K,V> *current_level_head = head;
        while (p) { // find p
            while(p->right && p->right->key < key)
                p = p->right;
            if (p->right && p->right->key == key) {
                p = p->right;
                break;
            }
            p = p->down;
            current_level_head = head->down;
        }
        if (!p)
            return false;
        
        // delete and link
        Node<K,V> *tmp = p;
        Node<K,V> *current_node = current_level_head;
        while (tmp) {
            tmp = p->down;
            // link
            if (current_level_head == head && head->right->right == NULL) // delete at the top level
                head = head->down;
            while (current_node->right != p)
                current_node = current_node->right;
            current_node->right = p->right;
            current_level_head = current_level_head->down;
            current_node = current_level_head;
            // delete
            free(p);
            p = tmp;
        }
        num_of_nodes--;
        return true;
    }
};

int main() {
    Skiplist<int, int> sl;
    sl.put(1, 1);
    sl.put(4, 3);
    sl.put(2, 2);
    cout << *(sl.get(4)) << endl;
    sl.remove(2);
    sl.put(5, 2);
    sl.put(0, 2);
    sl.put(0, 1);
    cout << sl.size() << endl;
    cout << *(sl.get(1)) << endl;
    cout << *(sl.get(4)) << endl;
    cout << *(sl.get(5)) << endl;
    sl.remove(5);
    cout << *(sl.get(0)) << endl;
    return 0;
}