/* description: https://202.120.40.8:30463/contest/10/problem/1
 * start: 2021-4-29
 * timelimit: 6days
 */

#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

string GetPalindromeString(string s) {
    string origin_s = s;
    reverse(s.begin(), s.end());

    if (origin_s == s || s == "")
        return s;

    int len = s.length() - 1;
    int i = len;
    while (i >= 0) {
        int ii = i, jj = len;
        while (ii >= 0) {
            if (origin_s[ii] != s[jj])
                break;
            ii--;
            jj--;
        }
        if (ii < 0)  // find the max fit length
            break;

        i--;
    }
    string result = s.substr(0, len - i) + origin_s;
    return result;
}


/*
int main() {
    string s;
    cin >> s;
    cout << GetPalindromeString(s) << endl;
    return 0;
}
*/