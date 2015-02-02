#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <memory.h>
#include <iostream>
#include <cstdio>
#include <cmath>
#define SENTINEL (-1)
#define EDIT_COST (1)
using namespace std;
inline int min(int a, int b) {
    return a < b ? a : b;
}
int Minimum(int a, int b, int c) {
    return min(min(a, b), c);
}
int EditDistanceDP(string & x,string & y) {
    int cost = 0;
    int leftCell, topCell, cornerCell;
    int m = x.length() + 1;
    int n = y.length() + 1;
    int table[m][n];
    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
            table[i][j] = SENTINEL;
    for(int i = 0; i < m; i++)
        table[i][0] = i;
    for(int j = 0; j < n; j++)
        table[0][j] = j;
    for(int i = 1; i < m; i++) {
        for(int j = 1; j < n; j++) {
            leftCell = table[i][j-1];
            leftCell += EDIT_COST;
            topCell = table[i-1][j];
            topCell += EDIT_COST;
            cornerCell = table[i-1][j-1];  //cost == 0
            cornerCell += (x[i-1] != y[j-1]);  //cost  == 1
            table[i][j] = Minimum(leftCell, topCell, cornerCell);
        }
    }
    cost = table[m-1][n-1];
    return cost;
} 
int main() {
    string x,y;
    cin >>x; cin >>y;
    cout << EditDistanceDP(x,y);
    return 0;
}