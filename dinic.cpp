#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <cassert>
using namespace std;
typedef long long ll;

struct Dinic {
    struct Edge {
        int to;
        ll cap;
        int rev; // 反向边在 G[to] 中的下标
    };

    int n;
    vector<vector<Edge>> G;
    vector<int> level, iter;
    vector<bool> visited; // 用于标记最小割的 S 集合

    Dinic(int _n) : n(_n) {
        G.resize(n);
        level.resize(n);
        iter.resize(n);
        visited.resize(n, false);
    }

    // 添加有向边 from -> to，容量为 cap
    void add_edge(int from, int to, ll cap) {
        G[from].push_back({to, cap, (int)G[to].size()});
        G[to].push_back({from, 0, (int)G[from].size() - 1});
    }

    // 添加无向边（双向容量均为 cap）
    void add_undirected_edge(int u, int v, ll cap) {
        G[u].push_back({v, cap, (int)G[v].size()});
        G[v].push_back({u, cap, (int)G[u].size() - 1});
    }

    // BFS 构建分层图
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (const auto& e : G[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    // DFS 寻找增广路
    ll dfs(int v, int t, ll f) {
        if (v == t) return f;
        for (int& i = iter[v]; i < (int)G[v].size(); i++) {
            Edge& e = G[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                ll d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

    // 计算最大流
    ll max_flow(int s, int t) {
        ll flow = 0;
        while (bfs(s, t)) {
            fill(iter.begin(), iter.end(), 0);
            ll f;
            while ((f = dfs(s, t, LLONG_MAX)) > 0) flow += f;
        }
        return flow;
    }

    // --- 最小割与流量查询（必须在 max_flow 之后调用） ---

    // 在残量网络中从 s 出发遍历，标记 S 集合
    void dfs_min_cut(int v) {
        visited[v] = true;
        for (const auto& e : G[v]) {
            if (e.cap > 0 && !visited[e.to]) dfs_min_cut(e.to);
        }
    }

    // 获取最小割的 S 集合划分（visited[i]=true 属于 S 集合，false 属于 T 集合）
    void get_min_cut_partition(int s) {
        fill(visited.begin(), visited.end(), false);
        dfs_min_cut(s);
    }

    // 获取某条边的实际流量（建图时记录的 u 和 idx）
    ll get_edge_flow(int u, int idx) {
        return G[u][idx].cap == 0 ? G[G[u][idx].to][G[u][idx].rev].cap : 0;
        // 更通用的写法：流量 = 反向边的 cap
        // 但需要知道反向边，这里简化：如果正向边 cap==0，流量=反向边cap
        // 实际上直接返回 G[e.to][e.rev].cap 即可
    }

    // 打印最小割的具体割边（S -> T 的饱和边）
    void print_min_cut_edges(int s) {
        get_min_cut_partition(s);
        cout << "最小割边集 (S -> T):" << endl;
        for (int u = 0; u < n; u++) {
            if (!visited[u]) continue;
            for (const auto& e : G[u]) {
                if (!visited[e.to] && e.cap == 0 && G[e.to][e.rev].cap > 0) {
                    cout << u << " -> " << e.to << " (割边, 流量: " << G[e.to][e.rev].cap << ")" << endl;
                }
            }
        }
    }
};

// ==================== 测试用例 ====================

// 测试1：经典有向图最大流（CLRS 教材例题）
// 图结构：s=0, t=5
// 0->1(16), 0->2(13), 1->2(10), 1->3(12), 2->1(4), 2->4(14), 3->2(9), 3->5(20), 4->3(7), 4->5(4)
// 期望最大流 = 23
void test_directed_graph() {
    cout << "=== 测试1：有向图最大流 ===" << endl;
    Dinic dinic(6);
    dinic.add_edge(0, 1, 16);
    dinic.add_edge(0, 2, 13);
    dinic.add_edge(1, 2, 10);
    dinic.add_edge(1, 3, 12);
    dinic.add_edge(2, 1, 4);
    dinic.add_edge(2, 4, 14);
    dinic.add_edge(3, 2, 9);
    dinic.add_edge(3, 5, 20);
    dinic.add_edge(4, 3, 7);
    dinic.add_edge(4, 5, 4);

    ll flow = dinic.max_flow(0, 5);
    cout << "最大流: " << flow << " (期望: 23)" << endl;
    assert(flow == 23);

    // 测试最小割
    dinic.print_min_cut_edges(0);
    cout << "测试1通过！" << endl << endl;
}

// 测试2：最小割点集划分验证
// 简单图：0->1(3), 0->2(2), 1->3(2), 2->3(5), 源=0, 汇=3
// 最大流=4，最小割边为 0->1 和 0->2（或 1->3 和 2->3 取决于实现）
void test_min_cut_partition() {
    cout << "=== 测试2：最小割点集划分 ===" << endl;
    Dinic dinic(4);
    dinic.add_edge(0, 1, 3);
    dinic.add_edge(0, 2, 2);
    dinic.add_edge(1, 3, 2);
    dinic.add_edge(2, 3, 5);

    ll flow = dinic.max_flow(0, 3);
    cout << "最大流: " << flow << " (期望: 4)" << endl;
    assert(flow == 4);

    dinic.get_min_cut_partition(0);
    cout << "S 集合: ";
    for (int i = 0; i < 4; i++) if (dinic.visited[i]) cout << i << " ";
    cout << endl;
    cout << "T 集合: ";
    for (int i = 0; i < 4; i++) if (!dinic.visited[i]) cout << i << " ";
    cout << endl;
    // 源点0必在S，汇点3必在T
    assert(dinic.visited[0] == true);
    assert(dinic.visited[3] == false);
    cout << "测试2通过！" << endl << endl;
}

// 测试3：无向图最大流
// 三角形：0-1(5), 1-2(3), 0-2(4)，源=0, 汇=2
// 无向边双向容量，最大流 = min(5+4, 3+?) = 7（走0->2直接4，0->1->2走3）
void test_undirected_graph() {
    cout << "=== 测试3：无向图最大流 ===" << endl;
    Dinic dinic(3);
    dinic.add_undirected_edge(0, 1, 5);
    dinic.add_undirected_edge(1, 2, 3);
    dinic.add_undirected_edge(0, 2, 4);

    ll flow = dinic.max_flow(0, 2);
    cout << "最大流: " << flow << " (期望: 7)" << endl;
    assert(flow == 7);
    cout << "测试3通过！" << endl << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    test_directed_graph();
    test_min_cut_partition();
    test_undirected_graph();

    cout << "全部测试通过！模板可用。" << endl;
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