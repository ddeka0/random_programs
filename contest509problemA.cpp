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
 
using namespace std;

int main() {
	int n;
	int max = 1;
	cin >> n;
	int arr[n][n];
	for(int i=0;i<n;i++) {
		for(int j=0;j<n;j++) {
			if( (i == 0) || (j == 0) )
				arr[i][j] = 1;
			else {
				arr[i][j] = arr[i][j-1] + arr[i-1][j];
				if(arr[i][j] > max)
					max = arr[i][j];
			}

		}
	}
	cout << max;
	return 0;
}