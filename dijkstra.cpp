#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const ll LINF = 1e18; // 根据题目数据范围调整，防止溢出

struct Dijkstra {
    vector<vector<pii>> adj; // 邻接表，存储 {to, weight}
    vector<ll> dist;         // dist[i] 表示源点到 i 的最短距离

    Dijkstra(int n) : adj(n), dist(n, LINF) {}

    void add_edge(int u, int v, ll w) {
        adj[u].pb({v, w});
    }

    /*
     * ================== Dijkstra 算法 (堆优化) ==================
     * 【核心思想】
     * 贪心策略：每次从尚未确定最短路的点中，选出距离源点最近的点 u，
     * 将其标记为已确定，并用 u 去松弛（更新）其所有邻居的距离。
     * 
     * 【堆优化原理】
     * 使用小根堆（priority_queue + greater）维护当前所有“待扩展”节点的距离。
     * 每次取堆顶即为当前全局最小距离节点，避免了对所有节点进行 O(V) 的暴力扫描。
     * 
     * 【关键剪枝】
     * if (d > dist[u]) continue; 
     * 由于堆中可能残留同一个节点的旧距离（即已经被更短的路径更新过），
     * 取出堆顶时必须判断 d 是否等于当前的 dist[u]，若大于则说明是过期数据，直接跳过。
     * 
     * 【时间复杂度】
     * O((V + E) log V)，其中 V 为点数，E 为边数。
     * 
     * 【适用条件与限制】
     * 1. 仅适用于边权非负的图（w >= 0）。若存在负权边，必须改用 SPFA 或 Bellman-Ford。
     * 2. 无法处理负权环。
     * 
     * 【常见扩展用法】
     * 1. 多源最短路：将所有源点的 dist 初始化为 0，并全部压入堆中。
     * 2. 记录路径：增加一个 pre[] 数组，在松弛成功时记录 pre[v] = u。
     * 3. 次短路 / 第 K 短路：允许每个节点入堆多次，记录前 K 个不同的距离。
     * 4. 分层图最短路：将状态扩展为 (node, layer)，用于处理“免费走 k 条边”等限制。
     * ==============================================================
     */
    void run(int src) {
        // 小根堆，存储 {当前距离, 节点编号}
        priority_queue<pll, vector<pll>, greater<pll>> pq; 
        
        dist[src] = 0;
        pq.push({0, src});
        
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            
            // 关键剪枝：如果取出的距离大于已知的最短距离，说明是过期数据，跳过
            if (d > dist[u]) continue;
            
            // 遍历 u 的所有出边，进行松弛操作
            for (auto [v, w] : adj[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v}); // 将更新后的距离压入堆中
                }
            }
        }
    }
};

/*
 * ============================================================================================
 * File Description: C++ Template for Competitive Programming (CPC)
 * Template Source: https://github.com/moi9012/CPC-Template
 * Compiler Version: C++17 / C++20
 * Author Notes: Includes common headers, macros, fast I/O, and frequently used data structures.
 *               Suitable for platforms like Codeforces and CodeChef.
 * ============================================================================================
 */