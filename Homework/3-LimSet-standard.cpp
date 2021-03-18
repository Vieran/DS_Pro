//
// Created by N on 2021/3/18.
//

#include <list>
#include <unordered_map>

using namespace std;

/*
 *
 *                O(1) LimSet::remove()
 *                         |
 *      O(1) LimSet::add() |      O(1) pop_front()
 *               |         |              |
 *               |         |              |
 *      _order  key1 -> key2 -> key3 ->key4      std::list
 *                       /       |
 *                      |        |
 *       _map    ptr_to_key2 | ptr_to_key3        std::unordered_map
 *                  key2     |    key3
 *                           |
 *                           |
 *                 O(1) LimSet::in()
 */

template<typename K>
class LimSet {
private:
    /*
     * Always check if we update
     * private members in all APIs
     *
     * Implementing your own
     * std::list/unordered_map can improve performance
     */
    int size, cap;
    list<K> _order; // keep the order of usage for key
    unordered_map<K, typename list<K>::const_iterator> _map; // map key to its position in _order

public:
    explicit LimSet(int capacity) {
        size = 0;
        cap = capacity;
    }

    bool in(const K &key) {
        auto it = _map.find(key);
        if (it == _map.end())
            return false;

        _order.erase(it->second);             // erase from _order
        _order.push_front(key);                 // move to front
        _map[key] = _order.begin();             // update _map pointer to front
        return true;
    }

    void add(const K &key) {
        auto it = _map.find(key);

        if (it == _map.end())
            if (size == cap) {                  // no need to update size
                _map.erase(_order.back());      // erase _map pointer to back
                _order.pop_back();              // erase _order's back
            } else
                ++size;                         // update size
        else
            _order.erase(it->second);         // erase from _order

        _order.push_front(key);                 // in all cases, we must move key to front
        _map[key] = _order.begin();             // update _map pointer to front
    }

    void remove(const K &key) {
        auto it = _map.find(key);
        if (it == _map.end())
            return;

        _order.erase(it->second);             // erase from _order
        _map.erase(key);                        // erase _map pointer to back
        --size;                                 // update size
    }
};