/**
 * アルゴリズムイントロダクション章末問題 19-1「オフライン最小値」
 * （Offline minimium problem）
 *
 * 1以上n以下の整数を定義域とする要素の動的集合について、insertとextract_min操作が利用できるようにする
 * 　extract_min := 動的集合から最小値を取り出し、その値は動的集合から削除する
 * ちょうどn回のinsertと、m回のextract_min操作から構成される操作列Sがオフラインで与えられる
 * ただしinsertでは、1以上n以下の整数がちょうど1回ずつ現れる
 * extract_min操作で各操作が返すキーをn行出力せよ
 *
 * 入力：長さn+mの整数配列Aで、A_i = -1ならextact_min
 * 出力：長さnの配列を出力せよ
 *
 * 入力例
 * 4 8 -1 3 -1 9 2 6 -1 -1 -1 1 7 -1 5
 * 出力例
 * 4 3 2 6 8 1
 *
 * o(n log{n})。つまり、Θ(n log{n})よりも漸近的に高速なアルゴリズムを設計せよ
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

// アルゴリズムイントロダクション章末19-1
void test_offline_minimum_problem() {
  // -1以外はinsert、-1はextract_min
  vector<int> input = {3, 7, -1, 2, -1, 8, 1, 5, -1, -1, -1, 0, 6, -1, 4};
  vector<int> expected = {3, 2, 1, 5, 7, 0};
  //

  auto offline_minimum = [input](int m, int n) -> vector<int> {
    vector<int> extracted(m);
    Dsu dsu(9);

    // Union-FindのリーダからKにおけるindexを引く
    vector<int> leader_to_kj(n);
    // kjに所属する要素のうち1つの値（kjが空なら空集合）
    vector<int> kj_to_val(m + 1, -1);
    {
      int kj = 0;
      for (int i = 0; i < (int)input.size(); i++) {
        if (input[i] == -1) {
          kj++;
        } else {
          kj_to_val[kj] = input[i];
          leader_to_kj[input[i]] = kj;
        }
      }
    }
    // 列Sを均質な部分列に分割するパート
    {
      for (int i = 1; i < (int)input.size(); i++) {
        if (input[i - 1] != -1 && input[i] != -1) {
          dsu.unite(input[i - 1], input[i]);
        }
      }
    }
    // leader_to_kj: [ 5 2 1 0 6 2 5 0 2 ]
    debug(leader_to_kj);
    // kj_to_val: [ 7 2 5 -1 -1 6 4 ]
    debug(kj_to_val);

    for (int i = 0; i < n; i++) {
      // i ∈ K_jを満たすようなjを求める
      int j = leader_to_kj[dsu.get_leader(i)];
      if (j != m) {
        extracted[j] = i;
        // lをjより大きく、集合K_lが存在する整数の中で最小の値とする
        // K_l = K_j ∪ K_lとして、K_jを除去する
        leader_to_kj[dsu.get_leader(i)]++;
        if (kj_to_val[j + 1] != -1) {
          leader_to_kj[dsu.get_leader(i)] =
              leader_to_kj[dsu.get_leader(kj_to_val[j + 1])];
          dsu.unite(i, kj_to_val[j + 1]);
        }
      }
    }
    /*
      i: 0 | dsu.get_leader(i): 0 | j: 5
      i: 1 | dsu.get_leader(i): 8 | j: 2
      i: 2 | dsu.get_leader(i): 2 | j: 1
      i: 3 | dsu.get_leader(i): 3 | j: 0
      i: 4 | dsu.get_leader(i): 0 | j: 6
      i: 5 | dsu.get_leader(i): 3 | j: 3
      i: 6 | dsu.get_leader(i): 0 | j: 6
      i: 7 | dsu.get_leader(i): 3 | j: 4
      i: 8 | dsu.get_leader(i): 3 | j: 6
     */
    // extracted: [ 3 2 1 5 7 0 ]
    return extracted;
  };

  vector<int> actual = offline_minimum(6, 9);
  assert(expected == actual);
}

int main() { test_offline_minimum_problem(); }