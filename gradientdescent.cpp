/* Debashish Deka*/
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
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;
/*..................................................
sample text file containing 10 cases
10
1 1  // xi[0] and yi[0]
2 2  // xi[1] and yi[1]
3 3  // and so on...
4 4
5 5
6 6
7 7
8 8
9 9
10 10
.....................................................*/

int main() {
	unsigned long long iteration = 0;
	int total_case; float m = 0;
	cin >> total_case;
	int *xi = new int[total_case];  int *yi = new int[total_case];
	m = total_case;
	// take the inputs into two arrays
	for(int i=0;i<total_case;i++) {
		cin >> xi[i]>>yi[i];
	}
	double theta0 = -10.0,theta1 = 5.0; // initial assumptions
	double k1=0,k2=0; // required terms in the equations
	double error_theta0 = 10000000, error_theta1 = 10000000; // initial large vales are taken
	double learning_rate = 0.005; // can be changed ... lower the learning_rate if the solutions diverged
	for(int i=0;i<total_case;i++) {
		k1 = k1 + learning_rate*(1/m)*(theta0 + theta1*xi[i] - yi[i]);  // m = total_case
		k2 = k2 + learning_rate*(1/m)*(theta0 + theta1*xi[i] - yi[i])*xi[i]; // m = total_case
	}
	double theta1_prev,theta0_prev;
	while((error_theta0 > 0.0000001) || (error_theta1 > 0.0000001)) {
		iteration++;	
		theta0_prev = theta0; theta1_prev = theta1;
		theta0 = theta0 - k1; // eqation 1 // simultaneous update with old values of theta0 and theta1
		theta1 = theta1 - k2; // eqation 2 // simultaneous update with old values of theta0 and theta1
		k1 = 0;k2 = 0;
		for(int i=0;i<total_case;i++) { // calculations of terms using new values of theta0 and theta1
			k1 = k1 + learning_rate*(1/m)*(theta0 + theta1*xi[i] - yi[i]);
			k2 = k2 + learning_rate*(1/m)*(theta0 + theta1*xi[i] - yi[i])*xi[i];
		}
		error_theta0 = abs(theta0 - theta0_prev); error_theta1 = abs(theta1 - theta1_prev);
		cout <<"Iteration "<<iteration<<"   Theta0 = "<<theta0 <<"  Theta1 = "<<theta1<<endl;
	}
	/*cout <<"Theta0 = "<<theta0 <<"  Theta1 = "<< theta1<<endl;*/ cout <<"Y = "<<theta0<<" + "<<theta1<<" * "<<"X"<<endl;
	return 0;
}