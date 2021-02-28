/* description: https://202.120.40.8:30463/contest/2/problem/1
 * start: 2021-2-27
 * timelimit: 4days
 */

template <typename K, typename V> 
struct Dictionary {
    int size(){
        return 0;
    }
    bool put( K, V ){
        return true;
    }
    V* get( K k ){
        return nullptr;
    }
    bool remove( K k ) {
        return true;
    }
    template <typename K1, typename V1> 
    friend ostream& operator<<(ostream& os, const Dictionary<K1,V1>& dic);
};

template <typename K1, typename V1> 
ostream& operator<<(ostream& os, const Dictionary<K1,V1>& dic)
{
    return os;
}

