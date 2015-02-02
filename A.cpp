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
bool arr[1000][1000];
int main() {
	int n,m,k,x,y,flag = 1;
	cin >> n>>m>>k;
	for(int i=0;i<k;i++) {
		cin >> x>>y;
		x = x-1;y = y-1;
		arr[x][y] = true;
		if( ( (y+1)<m && (x>0) ) && ( (arr[x-1][y] ==true) && (arr[x-1][y+1] ==true) && (arr[x][y+1] ==true) && (arr[x][y] ==true) ) ) {
			printf("%d\n",i+1);
			flag = 0;
			break;
		}
		else if( ( (x>0) && (y>0) ) && ( (arr[x][y-1] ==true) && (arr[x-1][y-1] ==true) && (arr[x-1][y] ==true) && (arr[x][y] ==true) ) ) {
			printf("%d\n",i+1);
			flag = 0;
			break;
		}
		else if( ( (x+1)<n && (y>0) ) && ( (arr[x+1][y-1] ==true) && (arr[x][y-1] ==true) && (arr[x+1][y] ==true) && (arr[x][y] ==true) ) ) {
			printf("%d\n",i+1);
			flag = 0;
			break;
		}
		else if( ( (x+1)<n && (y+1)<m )  && ( (arr[x][y+1] ==true) && (arr[x+1][y+1] ==true) && (arr[x+1][y] ==true) && (arr[x][y] ==true) ) ) {
			printf("%d\n",i+1);
			flag = 0;
			break;
		}
	}

	if(flag == 1)
		printf("%d\n",0);

	return 0;

}