/**
 * アルゴリズムイントロダクション練習問題
 * グループの要素列挙
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
 private:
  vector<int> next;  // get_group用

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
    next.resize(n);
    rep(i, n) {
      leader[i] = i;
      parent[i] = i;
      next[i] = i;
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
    swap(next[x], next[y]);
  }

  bool is_same(int x, int y) { return get_leader(x) == get_leader(y); }

  // 要素xが所属するグループを列挙する(ソートはされていない)
  vector<int> get_group(int x) {
    vector<int> ret = {x};
    int y = x;
    while (next[y] != x) {
      y = next[y];
      ret.push_back(y);
    }
    return ret;
  }
};

void test_print_group() {
  Dsu dsu(10);

  auto is_same_vec = [](vector<int> vec1, vector<int> vec2) -> bool {
    sort(all(vec1));
    sort(all(vec2));
    return vec1 == vec2;
  };

  assert(is_same_vec(dsu.get_group(0), vector<int>{0}));
  dsu.unite(0, 1);
  dsu.unite(0, 2);
  dsu.unite(1, 3);
  assert(is_same_vec(dsu.get_group(0), vector<int>{0, 1, 2, 3}));
  assert(!is_same_vec(dsu.get_group(5), vector<int>{4, 5}));
  dsu.unite(4, 5);
  assert(is_same_vec(dsu.get_group(5), vector<int>{4, 5}));
  dsu.unite(2, 5);
  assert(is_same_vec(dsu.get_group(5), vector<int>{0, 1, 2, 3, 4, 5}));
}

int main() { test_print_group(); }