#include <iostream>
using ostream = std::ostream;

// Copy the following codes to oj
#include <map>
template <typename K, typename V> 
struct Dictionary {
    std::map<K, V> _dict;
    int size(){
        return _dict.size();
    }
    bool put( K k, V v ){
        _dict[k] = v;
        return true;
    }
    V* get( K k ){
        if (_dict.find(k) != _dict.end()){
            return &_dict[k];
        } else {
            return nullptr;
        }
    }
    bool remove( K k ) {
        auto pos = _dict.find(k);
        if (pos != _dict.end()){
            // cout << "pos:" << pos << endl; 
            _dict.erase(pos);
            return true;
        } else {   
            return false;
        }
    }
    template <typename K1, typename V1> 
    friend ostream& operator<<(ostream& os, const Dictionary<K1,V1>& dic);
};

template <typename K1, typename V1> 
ostream& operator<<(ostream& os, const Dictionary<K1,V1>& dic)
{
    for (auto &iter:dic._dict)
         os << iter.first << " " << iter.second << " ";
    return os;
}
// Copy code ends.

int main() {
    using namespace std;
    Dictionary<int, string> dict;
    cout << "Size:" << dict.size() << endl;
    dict.put(1, "1");
    dict.put(9, "10");
    dict.put(5, "5");
    dict.put(100, "100");
    cout << "Size:" << dict.size() << endl;
    cout << "Dict:" << dict << endl;
    for (int i = 0; i< 10; i++) {
        if (dict.get(i))
            cout << "Dict.get(" << i << ")" << " = " << *dict.get(i) << endl;
        else   
            cout << "Dict.get(" << i << ")" << " not found" << endl;
    }
    for (int i = 0; i< 10; i++) {
        if (dict.remove(i))
            cout << "Dict.revome(" << i << ")" << " success " << endl
                 << "remain:" << dict << "(size=" << dict.size() << endl;
        else   
            cout << "Dict.remove(" << i << ")" << "failed" << endl;
    }
    cout << "Final remain:" << dict << "(size=" << dict.size() << endl;
    return 0;
}