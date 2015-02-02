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

int main() {
  freopen("in", "r", stdin);
  freopen("out", "w", stdout);
  int tt;
  scanf("%d", &tt);
  for (int qq = 1; qq <= tt; qq++) {
    printf("Case #%d: ", qq);
    string s;
    cin >> s;
    int len = s.length();
    int bmin, bmax;
    stringstream ss;
    ss << s;
    ss >> bmin;
    bmax = bmin;
    for (int i = 0; i < len; i++) {
      for (int j = 0; j < len; j++) {
        swap(s[i], s[j]);
        if (len == 1 || s[0] != '0') {
          stringstream ww;
          ww << s;
          int num;
          ww >> num;
          if (num < bmin) bmin = num;
          if (num > bmax) bmax = num;
        }
        swap(s[i], s[j]);
      }
    }
    printf("%d %d\n", bmin, bmax);
  }
  return 0;
}