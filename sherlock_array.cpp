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
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string.h>
using namespace std;
int main() {
	int tt,n,x,total; bool flag = false;
	cin >>tt;
	for(int i=0;i<tt;i++) {
		cin >> n;
		int sum[n];
		memset(sum,0,n);
		total = 0; flag = false;		
		for(int i=0;i<n;i++) {
			cin >>x;
			if(i == 0)
				sum[i] = x;
			else 
				sum[i] = sum[i-1] + x;
			total +=x;
		}
		if(n == 2) {
			cout << "NO"<<endl;
			flag = true;
		}
		else if(n == 1) {
			cout << "YES"<<endl;
			flag = true;	
		}
		else {
			for(int i=1;i<n-1;i++) {
				if(sum[i-1] == (total - sum[i])) {
					cout << "YES"<<endl;
					flag = true;
					break;
				}

			}
		}
		if(flag == false)
			cout << "NO"<<endl;

	}
	return 0;
}