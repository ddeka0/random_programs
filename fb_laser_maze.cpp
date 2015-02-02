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

const int dx[4] = {0, 1, 0, -1};
const int dy[4] = {1, 0, -1, 0};
const string let = ">v<^";
const int N = 444;
const int MAX = N * N * 5;

bool was[N][N][5];
int x[MAX], y[MAX], z[MAX], d[MAX];
char s[N][N];

int main() {
  freopen("in", "r", stdin);
  freopen("out", "w", stdout);
  int tt;
  scanf("%d", &tt);
  for (int qq = 1; qq <= tt; qq++) {
    printf("Case #%d: ", qq);
    int h, w;
    scanf("%d %d", &h, &w);
    for (int i = 0; i < h; i++) {
      scanf("%s", s[i]);
    }
    for (int rot = 0; rot < 4; rot++) {
      for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
          was[i][j][rot] = (s[i][j] == '#');
        }
      }
      for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
          int pos = let.find(s[i][j]);
          if (pos != string::npos) {
            int x = i, y = j;
            while (0 <= x && x < h && 0 <= y && y < w && s[x][y] != '#' && ((x == i && y == j) || let.find(s[x][y]) == string::npos)) {
              was[x][y][rot] = true;
              x += dx[pos];
              y += dy[pos];
            }
            s[i][j] = let[(pos + 1) % let.length()];
          }
        }
      }
    }
    int b = 0, e = 0;
    for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
        if (s[i][j] == 'S') {
          x[0] = i;
          y[0] = j;
        }
      }
    }
    z[0] = 0;
    was[x[0]][y[0]][z[0]] = true;
    d[0] = 0;
    int ans = -1;
    while (b <= e) {
      if (s[x[b]][y[b]] == 'G') {
        ans = d[b];
        break;
      }
      for (int q = 0; q < 4; q++) {
        int xk = x[b] + dx[q];
        int yk = y[b] + dy[q];
        int zk = (z[b] + 1) % 4;
        if (0 <= xk && xk < h && 0 <= yk && yk < w) {
          if (was[xk][yk][zk]) {
            continue;
          }
          e++;
          x[e] = xk;
          y[e] = yk;
          z[e] = zk;
          was[xk][yk][zk] = true;
          d[e] = d[b] + 1;
        }
      }
      b++;
    }
    if (ans == -1) {
      puts("impossible");
    } else {
      printf("%d\n", ans);
    }
  }
  return 0;
}