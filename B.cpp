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
#include <string>

using namespace std;
string s;
int main() {	
    		getline(cin,s);
    		int len = s.length();
    		int flag = 1,cnt=0;
            for(int i=0;i<len;i++) {
                if( (s[i] - '0')%2 == 0 ) {
                    cnt++;
                }
            }
            if(cnt == len) {
                cout << -1;
                return 0;
            }
    		for(int i=0;i<len-1,flag !=0;i++) {
    			if( (s[i] - '0')%2 == 0 )  {
                    if( int(s[i]) < int(s[len - 1]) ) {
                        swap(s[i],s[len - 1]);
                        cout << s;
                        cout << "Here";

                        return 0;                        
                    }

                }
            }
            cout << "Here";
            for(int i=len - 1;i>0,flag !=0;i--) {
                if( (s[i] - '0')%2 == 0 )   {
                    if(s[i] > s[len - 1]) {
                        swap(s[i],s[len - 1]);
                        cout << s;
                        return 0;                        
                    }

                }
            }

}
