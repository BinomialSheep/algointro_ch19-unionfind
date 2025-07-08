/**
 * アルゴリズムイントロダクション章末問題 19-2「深さの決定」
 * （depth-determination problem)
 *
 * find_depth(v)：vが属する木におけるvの深さを返す
 * graft(r,v)：節点rを節点vの子にする。ただしrは木の根、vはrとは別の木の（根とは限らない）節点
 *
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
  // 親ノードとの差分
  vector<int> diff_weight;

  Dsu() {}
  Dsu(int n_) : n(n_) {
    leader.resize(n);
    parent.resize(n);
    rank.resize(n);
    diff_weight.resize(n);
    rep(i, n) {
      leader[i] = i;
      parent[i] = i;
      rank[i] = 0;
      diff_weight[i] = 0;
    }
  }
  int get_leader(int x) {
    if (parent[x] == x) return x;
    int root = get_leader(parent[x]);
    diff_weight[x] += diff_weight[parent[x]];
    return parent[x] = root;
  }

  // weight(y) - weight(x) = wとなるようにマージする
  void unite(int x, int y, int w) {
    w = w + get_weight(x) - get_weight(y);

    int lx = get_leader(x);
    int ly = get_leader(y);
    assert(lx != ly);
    if (rank[lx] == rank[ly]) {
      parent[ly] = lx;
      rank[lx]++;
      diff_weight[ly] = w;
    } else if (rank[lx] > rank[ly]) {
      parent[ly] = lx;
      diff_weight[ly] = w;
    } else {
      parent[lx] = ly;
      diff_weight[lx] = -w;
    }
  }

  int get_weight(int x) {
    // マージ後の不整合を経路圧縮しながら解消
    get_leader(x);
    return diff_weight[x];
  }

  bool is_same(int x, int y) { return get_leader(x) == get_leader(y); }
};

void test_depth_determination_problem() {
  Dsu dsu(10);

  vector<tuple<int, int, int>> query;
  query = {{2, 0, 1}, {1, 0, -1}, {1, 1, -1}, {2, 3, 2},
           {2, 2, 0}, {1, 2, -1}, {1, 3, -1}};
  vector<int> expected = {1, 0, 2, 3};

  vector<int> actual;
  rep(qi, query.size()) {
    auto [com, v1, v2] = query[qi];
    if (com == 1) {
      dsu.get_leader(v1);
      actual.push_back(dsu.get_weight(v1));
    } else {
      dsu.unite(v2, v1, 1);
    }
    rep(i, 4) {
      //
      debug(i, dsu.get_weight(i));
    }
  }
  debug(actual);
  assert(expected == actual);
}

// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_1_B&lang=ja
void solve_weighted_union_find() {
  int n, q;
  cin >> n >> q;
  Dsu dsu(n);

  while (q--) {
    int com;
    cin >> com;
    if (com == 0) {
      int x, y, z;
      cin >> x >> y >> z;
      if (!dsu.is_same(x, y)) {
        dsu.unite(x, y, z);
      }
    } else {
      int x, y;
      cin >> x >> y;
      if (dsu.is_same(x, y)) {
        cout << dsu.get_weight(y) - dsu.get_weight(x) << endl;
      } else {
        cout << "?" << endl;
      }
    }
  }
}
int main() {
  // test_depth_determination_problem();
  solve_weighted_union_find();
}