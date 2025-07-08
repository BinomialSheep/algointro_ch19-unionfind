/**
 * アルゴリズムイントロダクション章末問題
 19-3「Tarjanのオフライン最小共通祖先アルゴリズム」
 * offline lowest common ancestor
 */

#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define all(x) (x).begin(), (x).end()
using ll = long long;
#ifdef LOCAL
#include <debug_print.hpp>
#define debug(...) debug_print::multi_print(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) (static_cast<void>(0))
#endif

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

void solve_grl_5_C() {
  int n;
  cin >> n;
  vector<vector<int>> graph(n);
  rep(i, n) {
    int k;
    cin >> k;
    rep(j, k) {
      int c;
      cin >> c;
      graph[i].push_back(c);
    }
  }
  int q;
  cin >> q;
  vector<pair<int, int>> query(q);
  rep(i, q) { cin >> query[i].first >> query[i].second; }
  // LCAを求めたいペアの集合(u, v)を、vec[u], vec[v]で列挙できるようにしておく
  vector<vector<int>> vec(n);
  for (auto [u, v] : query) {
    vec[u].push_back(v);
    vec[v].push_back(u);
  }
  map<pair<int, int>, int> mp;

  Dsu dsu(n);
  vector<int> ancestor(n);
  const int WHITE = 0, BLACK = 1;
  vector<int> color(n, WHITE);

  auto lca = [&](auto rec, int u) -> void {
    ancestor[dsu.get_leader(u)] = u;
    for (auto v : graph[u]) {
      rec(rec, v);
      dsu.unite(v, u);
      ancestor[dsu.get_leader(u)] = u;
    }
    color[u] = BLACK;
    for (auto v : vec[u]) {
      if (color[v] == BLACK) {
        // debug(u, v);
        mp[make_pair(min(v, u), max(v, u))] = ancestor[dsu.get_leader(v)];
      }
    }
  };

  lca(lca, 0);
  // debug(mp);

  rep(i, q) {
    auto [u, v] = query[i];
    if (u > v) swap(u, v);
    cout << mp[make_pair(u, v)] << endl;
  }
}

int main() { solve_grl_5_C(); }