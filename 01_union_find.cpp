#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define all(x) (x).begin(), (x).end()
using ll = long long;

class Dsu {
 public:
  int n;
  vector<int> leader;
  vector<int> parent;
  vector<int> rank;

  Dsu() {}
  Dsu(int n_) : n(n_) {
    leader.resize(n);
    parent.resize(n);
    rank.resize(n);
    rep(i, n) {
      leader[i] = i;
      parent[i] = i;
      rank[i] = 0;
    }
  }
  int get_leader(int x) {
    if (parent[x] == x) return x;
    return parent[x] = get_leader(parent[x]);
  }

  void unite(int x, int y) {
    x = get_leader(x);
    y = get_leader(y);
    if (x == y) return;
    if (rank[x] == rank[y]) {
      parent[y] = x;
      rank[x]++;
    } else if (rank[x] > rank[y]) {
      parent[y] = x;
    } else {
      parent[x] = y;
    }
  }

  bool is_same(int x, int y) { return get_leader(x) == get_leader(y); }
};

int main() {
  int N;
  cin >> N;
  Dsu dsu(N);
  int Q;
  cin >> Q;
  while (Q--) {
    int com, x, y;
    cin >> com >> x >> y;
    if (com == 0) {
      dsu.unite(x, y);
    } else {
      cout << (dsu.is_same(x, y) ? 1 : 0) << endl;
    }
  }
}