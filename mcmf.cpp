#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
#include <climits>

using namespace std;

typedef long long ll;

/*
 * ================== 最小费用最大流 (MCMF) 模型 ==================
 * 【问题本质】
 * 在保证流量达到最大的前提下，求整个网络的最小总花费（每条边除了容量，还多了一个单位流量费用）。
 * 
 * 【核心算法】
 * SSP (连续最短路算法)：每次在残量网络中寻找单位费用最小的增广路进行增广。
 * 本模板采用 SPFA 求最短路 + 类似 Dinic 的 DFS 多路增广优化。
 * 注意：图中绝对不能存在负权环，否则 SPFA 会死循环。
 * 
 * 【常见建图套路】
 * 1. 带权二分图最佳匹配：源点连左部点，右部点连汇点（容量1，费用0），中间连实际关系边。
 *    若求最大权匹配，将费用取反（cost = -w），跑最小费用流，最后结果取相反数。
 * 2. 资源分配/调度：通过拆点或分层图，将时间/空间维度展开，用费用流控制最优分配代价。
 * =================================================================
 */

struct Edge {
    int to;
    ll cap;
    ll cost; // 单位流量费用
    int rev; // 反向边下标
};

struct MCMF {
    vector<vector<Edge>> G;
    vector<ll> dist;
    vector<int> iter;
    vector<bool> vis;
    int n;

    MCMF(int n) : n(n) {
        G.resize(n);
        dist.resize(n);
        iter.resize(n);
        vis.resize(n);
    }

    // 添加边：from, to, 容量, 单位费用
    void add_edge(int from, int to, ll cap, ll cost) {
        G[from].push_back({to, cap, cost, (int)G[to].size()});
        G[to].push_back({to, 0, -cost, (int)G[from].size() - 1}); // 反向边容量为0，费用为负
    }

    // SPFA 寻找最短费用路径
    bool spfa(int s, int t) {
        fill(dist.begin(), dist.end(), LLONG_MAX);
        fill(vis.begin(), vis.end(), false);
        queue<int> q;
        dist[s] = 0;
        q.push(s);
        vis[s] = true;

        while (!q.empty()) {
            int v = q.front(); q.pop();
            vis[v] = false;
            for (const auto& e : G[v]) {
                if (e.cap > 0 && dist[v] != LLONG_MAX && dist[e.to] > dist[v] + e.cost) {
                    dist[e.to] = dist[v] + e.cost;
                    if (!vis[e.to]) {
                        vis[e.to] = true;
                        q.push(e.to);
                    }
                }
            }
        }
        return dist[t] != LLONG_MAX;
    }

    // DFS 多路增广（类 Dinic 优化）
    ll dfs(int v, int t, ll f) {
        if (v == t) return f;
        vis[v] = true; // 防止在 DFS 中走环
        ll res = 0;
        for (int& i = iter[v]; i < (int)G[v].size(); i++) {
            Edge& e = G[v][i];
            // 必须满足：有剩余容量，且满足最短路条件（dist[v] + cost == dist[to]），且未在 DFS 栈中
            if (e.cap > 0 && !vis[e.to] && dist[e.to] == dist[v] + e.cost) {
                ll d = dfs(e.to, t, min(f - res, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    res += d;
                    if (res == f) break; // 流量已满，提前退出
                }
            }
        }
        vis[v] = false; // 回溯时取消标记
        return res;
    }

    // 计算最小费用最大流
    pair<ll, ll> min_cost_max_flow(int s, int t) {
        ll flow = 0, cost = 0;
        while (spfa(s, t)) {
            fill(iter.begin(), iter.end(), 0);
            fill(vis.begin(), vis.end(), false);
            ll f;
            // 每次 SPFA 后，进行多路增广
            while ((f = dfs(s, t, LLONG_MAX)) > 0) {
                flow += f;
                cost += f * dist[t]; // 总费用 = 流量 * 汇点的最短距离
            }
        }
        return {flow, cost};
    }
};

// 示例用法 (对应洛谷 P3381 模板题)
int main() {
    int n, m, s, t;
    if (!(cin >> n >> m >> s >> t)) return 0;
    
    MCMF mcmf(n + 1); // 节点编号 1 到 n
    
    for (int i = 0; i < m; i++) {
        int u, v;
        ll cap, cost;
        cin >> u >> v >> cap >> cost;
        mcmf.add_edge(u, v, cap, cost);
    }
    
    auto res = mcmf.min_cost_max_flow(s, t);
    cout << res.first << " " << res.second << endl; // 输出最大流和最小费用
    
    return 0;
}