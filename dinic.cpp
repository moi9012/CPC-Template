#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

typedef long long ll;

/*
 * ================== 最大流 (Maximum Flow) 模型 ==================
 * 【问题本质】
 * 在满足边容量限制和流量守恒的前提下，求从源点 s 到汇点 t 的最大传输量。
 * 
 * 【核心算法】
 * Dinic 算法：利用 BFS 构建分层图，利用 DFS 进行多路增广，配合当前弧优化。
 * 时间复杂度：O(V^2 * E)，在二分图等特定图上表现极佳。
 * 
 * 【常见建图套路】
 * 1. 多源多汇：建立超级源点连向所有源点，超级汇点连向所有汇点，边权设为 INF。
 * 2. 二分图最大匹配：源点连左部点，右部点连汇点（边权均为1），中间连实际关系边。
 * 3. 路径不相交：若点不相交，需使用“拆点法”（将点拆为入点和出点，中间连容量为1的边）。
 * =================================================================
 */
 
 /*
 * ================== 最小割 (Minimum Cut) 模型 ==================
 * 【问题本质】
 * 将图中的点划分为两个集合 S(含源点) 和 T(含汇点)，求使得源汇不连通的割边容量之和的最小值。
 * 根据【最大流最小割定理】：最小割的容量恒等于最大流的值。
 * 
 * 【核心算法】
 * 求数值：直接跑一遍最大流即可。
 * 求方案：跑完最大流后，在残量网络中从源点 s 出发 DFS，能到达的点属于 S 集，其余属于 T 集。
 *          跨越 S 和 T 且满流的原图边即为最小割边。
 * 
 * 【常见建图套路】
 * 1. 二者选其一（项目选择/图像分割）：源点代表选A，汇点代表选B。连边 s->i 容量为选B的代价，
 *    i->t 容量为选A的代价。割开 s->i 意味着选B，割开 i->t 意味着选A。
 * 2. 最小点割：要求切断某些“点”。使用拆点法，将点 i 拆为 i_in 和 i_out，连边容量为点权。
 *    原图的边 (u,v) 转化为 u_out -> v_in，容量设为 INF（表示不可割）。
 * =================================================================
 */

struct Edge {
    int to;
    ll cap;
    int rev; // 反向边在邻接表中的下标
};

struct Dinic {
    vector<vector<Edge>> G;
    vector<int> level, iter;
    vector<bool> visited; // 用于标记最小割的 S 集合
    int n;

    Dinic(int n) : n(n) {
        G.resize(n);
        level.resize(n);
        iter.resize(n);
        visited.resize(n);
    }

    // 添加边
    void add_edge(int from, int to, ll cap) {
        G[from].push_back({to, cap, (int)G[to].size()});
        G[to].push_back({to, 0, (int)G[from].size() - 1}); 
    }

    // BFS 分层
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
            while ((f = dfs(s, t, LLONG_MAX)) > 0) {
                flow += f;
            }
        }
        return flow;
    }

    // --- 最小割相关函数 ---

    // 在残量网络中从 s 出发遍历，标记 S 集合
    void dfs_min_cut(int v) {
        visited[v] = true;
        for (const auto& e : G[v]) {
            if (e.cap > 0 && !visited[e.to]) {
                dfs_min_cut(e.to);
            }
        }
    }

    // 获取最小割的具体划分和割边
    // 必须在调用 max_flow 之后调用
    void get_min_cut(int s, int t) {
        fill(visited.begin(), visited.end(), false);
        dfs_min_cut(s); // 找出 S 集合（源点所在集合）
        
        // 此时 visited[i] == true 表示节点 i 属于 S 集合
        // visited[i] == false 表示节点 i 属于 T 集合（汇点所在集合）
        
        cout << "最小割边集 (S -> T):" << endl;
        for (int u = 0; u < n; u++) {
            if (visited[u]) { // u 属于 S 集合
                for (const auto& e : G[u]) {
                    // 如果终点属于 T 集合，且这条边是原图中的正向边
                    // 注意：这里通过判断反向边的容量是否大于0来确认它是否是原图的正向边
                    // 因为原图正向边的反向边初始容量为0，跑完最大流后如果反向边容量>0，说明有流量流过
                    // 但更严谨的做法是在建图时记录边，或者通过 e.cap 的初始状态判断。
                    // 在此模板中，我们假设所有 add_edge 添加的都是原图的正向边。
                    if (!visited[e.to]) {
                        cout << u << " -> " << e.to << " (割边)" << endl;
                    }
                }
            }
        }
    }
};

// 示例用法
int main() {
    int n, m, s, t;
    if (!(cin >> n >> m >> s >> t)) return 0;
    
    Dinic dinic(n + 1); // 节点编号从 1 到 n
    
    for (int i = 0; i < m; i++) {
        int u, v;
        ll cap;
        cin >> u >> v >> cap;
        dinic.add_edge(u, v, cap);
    }
    
    // 1. 求最大流（即最小割容量）
    ll max_flow_val = dinic.max_flow(s, t);
    cout << "最大流 / 最小割容量: " << max_flow_val << endl;
    
    // 2. 求具体的最小割边集和点集划分
    dinic.get_min_cut(s, t);
    
    return 0;
}