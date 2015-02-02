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

int a[42], b[42], c[42];

int main() {
  freopen("in", "r", stdin);
  freopen("out", "w", stdout);
  int tt;
  scanf("%d", &tt);
  for (int qq = 1; qq <= tt; qq++) {
    printf("Case #%d: ", qq);
    int x, y, z, n;
    scanf("%d %d %d", &x, &y, &z);
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
      scanf("%d %d %d", a + i, b + i, c + i);
    }
    bool ok = false;
    for (int t = 0; t < (1 << n); t++) {
      int p = 0, q = 0, r = 0;
      for (int i = 0; i < n; i++) {
        if (t & (1 << i)) {
          p += a[i];
          q += b[i];
          r += c[i];
        }
      }
      if (p == x && q == y && r == z) {
        ok = true;
      }
    }
    puts(ok ? "yes" : "no");
  }
  return 0;
}