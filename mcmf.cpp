#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;
typedef long long ll;

// ===================== 最小费用流模板 =====================
// 支持两种模式：
//   run(s, t, false) -> 最小费用最大流
//   run(s, t, true)  -> 最小费用流（只增广费用为负的路径，即最大收益）
// =========================================================

struct Edge {
    int to;      // 边的终点
    ll cap;      // 剩余容量
    ll cost;     // 单位流量费用
    int rev;     // 反向边在 G[to] 中的下标
};

struct MCMF {
    int n;
    vector<vector<Edge>> G;
    vector<ll> dist;
    vector<int> iter;
    vector<bool> inq;

    MCMF(int n) : n(n) {
        G.resize(n);
        dist.resize(n);
        iter.resize(n);
        inq.resize(n);
    }

    // 加有向边 from -> to，容量 cap，单位费用 cost
    void add_edge(int from, int to, ll cap, ll cost) {
        G[from].push_back({to, cap, cost, (int)G[to].size()});
        G[to].push_back({from, 0, -cost, (int)G[from].size() - 1});
    }

    // SPFA 求最短路（支持负权边，不支持负环）
    bool spfa(int s, int t) {
        fill(dist.begin(), dist.end(), LLONG_MAX);
        fill(inq.begin(), inq.end(), false);
        queue<int> q;
        dist[s] = 0;
        q.push(s);
        inq[s] = true;

        while (!q.empty()) {
            int v = q.front(); q.pop();
            inq[v] = false;
            for (auto& e : G[v]) {
                if (e.cap > 0 && dist[v] != LLONG_MAX
                    && dist[e.to] > dist[v] + e.cost) {
                    dist[e.to] = dist[v] + e.cost;
                    if (!inq[e.to]) {
                        inq[e.to] = true;
                        q.push(e.to);
                    }
                }
            }
        }
        return dist[t] != LLONG_MAX;
    }

    // DFS 多路增广，返回本次增广的流量
    ll dfs(int v, int t, ll f, ll& total_cost) {
        if (v == t) return f;
        for (int& i = iter[v]; i < (int)G[v].size(); i++) {
            Edge& e = G[v][i];
            if (e.cap > 0 && dist[v] != LLONG_MAX
                && dist[e.to] == dist[v] + e.cost) {
                ll d = dfs(e.to, t, min(f, e.cap), total_cost);
                if (d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    total_cost += d * e.cost;
                    return d;
                }
            }
        }
        return 0;
    }

    // 主函数
    // stop_on_positive = true  : 最小费用流（dist[t] >= 0 时停止，用于最大收益问题）
    // stop_on_positive = false : 最小费用最大流
    // 返回 {总流量, 总费用}
    pair<ll, ll> run(int s, int t, bool stop_on_positive = false) {
        ll flow = 0, cost = 0;
        while (spfa(s, t)) {
            if (stop_on_positive && dist[t] >= 0) break;
            fill(iter.begin(), iter.end(), 0);
            ll f;
            while ((f = dfs(s, t, LLONG_MAX, cost)) > 0) {
                flow += f;
            }
        }
        return {flow, cost};
    }
};

// ===================== 使用示例 =====================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 示例：N 行 N 列矩阵，每行每列最多选 K 个，最大化所选数之和
    int N, K;
    cin >> N >> K;
    vector<vector<ll>> A(N, vector<ll>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> A[i][j];

    // 节点编号：S=0, 行1~N, 列N+1~2N, T=2N+1
    int S = 0, T = 2 * N + 1;
    MCMF mcmf(T + 1);

    // 源点 -> 行，容量 K，费用 0
    for (int i = 1; i <= N; i++)
        mcmf.add_edge(S, i, K, 0);

    // 行 -> 列，容量 1，费用 -A[i][j]（取负转最小费用）
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            mcmf.add_edge(i, N + j, 1, -A[i - 1][j - 1]);

    // 列 -> 汇点，容量 K，费用 0
    for (int j = 1; j <= N; j++)
        mcmf.add_edge(N + j, T, K, 0);

    // 最大收益问题 -> 最小费用流，stop_on_positive = true
    auto [flow, cost] = mcmf.run(S, T, true);
    cout << -cost << '\n';   // 取负还原为最大收益

    return 0;
}

/*
 * ============================================================================================
 * File Description: C++ Template for Competitive Programming (CPC)
 * Template Source: https://github.com/moi9012/CPC-Template
 * Compiler Version: C++17 / C++20
 * Author Notes: Includes common headers, macros, fast I/O, and frequently used data structures.
 *               Suitable for platforms like Codeforces and CodeChef.
 * ============================================================================================
 */