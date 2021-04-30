/* description: https://202.120.40.8:30463/contest/9/problem/1
 * start: 2021-4-26
 * timelimit: 4days
 */

#include <iostream>
#include <vector>
#include <queue>
#include <string.h>
using namespace std;

struct index {
    int x, y;  // index of the grid
    int rest;  // block remain to destroy
    index(int _x, int _y, int _block) : x(_x), y(_y), rest(_block) {}
};


// return the least step from (0,0) to (m-1, n-1)
// k is the max num of blocks to destroy
int shortestPathInGrid(vector<vector<int>>& grid, int k) {
    int m = grid.size();  // get m
    int n = grid[0].size();  // get n
    if (m == 1 && n == 1)
        return 0;

    int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};  // direction
    bool visited[m][n][k+1];
    memset(visited, false, sizeof(visited));

    int step;
    queue<index> to_visit;
    to_visit.emplace(0, 0, k);
    visited[0][0][k] = true;
    for(step = 0; to_visit.size() > 0; step++) {
        int size_ = to_visit.size();
        for(int i = 0; i < size_; i++) {
            index cur = to_visit.front();
            to_visit.pop();
            for(int j = 0; j < 4; j++) {
                int dx = cur.x + dir[j][0];  // next position x
                int dy = cur.y + dir[j][1];  // next position y
                if(dx >= 0 && dy >= 0 && dx < m && dy < n) {  // in the grid
                    if(grid[dx][dy] == 0 && !visited[dx][dy][cur.rest]) {  // no block and this position is not visited yet
                        if(dx == m-1 && dy == n-1)  // reach destination
                            return step+1;
                        
                        to_visit.emplace(dx, dy, cur.rest);  // push this position into the queue
                        visited[dx][dy][cur.rest] = true;  // set it as visited
                    } else if (grid[dx][dy] == 1 && cur.rest > 0 && !visited[dx][dy][cur.rest-1]) {  // block and after blocked is not visited
                        to_visit.emplace(dx, dy, cur.rest-1);  // push this position into the queue
                        visited[dx][dy][cur.rest-1] = true;  // set it as visited
                    }
                }
            }
        }
    }
    return -1;
}
// how can this search the "shortest" path? it just search a posible path
// ---because it is a level search! each step takes the same weight
// remember no cheating

/*
int main() {
    vector<vector<int>> grid;
    vector<int> tmp = {0, 1, 1};
    grid.push_back(tmp);
    tmp = {1, 1, 1};
    grid.push_back(tmp);
    tmp = {1, 0, 0};
    grid.push_back(tmp);
    int k = 0;
    cout << shortestPathInGrid(grid, k) << endl;
    return 0;
}
*/