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
using namespace std;
int minm(int,int,int);
int main() {
	int tt,ii,m,n;
	cin >> tt;
	for(int ii=0;ii<tt;ii++) {
		cin >> m>>n;
		int cost_table[m][n],MCP_table[m][n];
		for(int i=0;i<m;i++) {
			for(int j=0;j<n;j++) {
				cin >> cost_table[i][j];
			}
		}
		MCP_table[0][0] = cost_table[0][0];
		for(int i=0;i<m;i++)
			MCP_table[i][0] = MCP_table[i-1][0] + cost_table[i][0];
		for(int i=0;i<n;i++)
			MCP_table[0][i] = MCP_table[0][i] + cost_table[0][i];
		for(int i=1;i<m;i++) {
			for(int j=1;j<n;j++) {
				MCP_table[i][j] = minm( MCP_table[i-1][j-1] , MCP_table[i][j-1] , MCP_table[i-1][j] );
			}
		}

		cout << MCP_table[2][2];
	}
	return 0;	
}
int minm(int x, int y, int z){
   if (x < y)
      return (x < z)? x : z;
   else
      return (y < z)? y : z;
}
