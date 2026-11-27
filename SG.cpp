#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <set>
#include <unordered_map>

using namespace std;

/**
 * 模块一：通用 Mex 计算函数
 * 作用：求一个集合的 minimum excludant (最小排斥值)
 * 复杂度：O(K) 或 O(K log K)，K 为集合大小
 */
int get_mex(const vector<int>& s) {
    // 优化：如果集合很大，可以用 bool 数组或 bitset 代替 set
    // 这里为了通用性，采用排序后扫描的方法，避免 set 的常数开销
    static bool vis[1005]; // 假设 SG 值不会超过 1000，根据题目调整
    memset(vis, 0, sizeof(vis));
    for (int x : s) {
        if (x < 1005) vis[x] = true;
    }
    int mex = 0;
    while (vis[mex]) mex++;
    return mex;
}

/**
 * 模块二：单堆取石子游戏 (打表找规律)
 * 适用场景：一堆石子，每次只能从给定的集合 S 中取数量
 * 注意：如果 N 很大，打表后必须手动找循环节 T，然后 N %= T
 */
namespace SinglePile {
    vector<int> S; // 可取的石子数集合
    vector<int> sg; // SG 值数组

    void init(const vector<int>& moves, int maxn = 1000) {
        S = moves;
        sg.assign(maxn, 0);
        for (int i = 1; i < maxn; i++) {
            vector<int> next_sg;
            for (int move : S) {
                if (i >= move) {
                    next_sg.push_back(sg[i - move]);
                }
            }
            sg[i] = get_mex(next_sg);
        }
    }

    // 获取 SG 值，如果 N 超出打表范围，需确保已找到循环节并取模
    int get_sg(int n) {
        return sg[n];
    }
}

/**
 * 模块三：DAG 图游戏 (多棋子移动)
 * 适用场景：棋子在有向无环图上移动，最后无法移动者输
 * 核心：每个节点的 SG 值 = mex{所有后继节点的 SG 值}
 * 最终答案：所有棋子所在节点 SG 值的异或和
 */
namespace DAGGame {
    const int MAXN = 1005; // 根据题目调整
    vector<int> adj[MAXN]; // 邻接表
    int memo[MAXN];        // 记忆化数组
    bool visited[MAXN];    // 访问标记

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    int get_sg(int u) {
        if (visited[u]) return memo[u];
        visited[u] = true;
        
        vector<int> next_sg;
        for (int v : adj[u]) {
            next_sg.push_back(get_sg(v));
        }
        return memo[u] = get_mex(next_sg);
    }

    // 解决多棋子问题：传入所有棋子所在的节点
    int solve(const vector<int>& pieces) {
        int total_xor = 0;
        for (int u : pieces) {
            total_xor ^= get_sg(u);
        }
        return total_xor; // 返回 0 则先手必败，否则先手必胜
    }
}

// 示例主函数
int main() {
    // --- 示例 1：单堆取石子 (每次取 1, 3, 4) ---
    // 规律：SG 值周期为 7 (0,1,0,1,2,0,1, 0,1,0,1,2,0,1...)
    SinglePile::init({1, 3, 4}, 100);
    // cout << "SG[5] = " << SinglePile::get_sg(5) << endl;

    // --- 示例 2：DAG 图游戏 ---
    // 建图：0 -> 1, 0 -> 2, 1 -> 3, 2 -> 3
    // DAGGame::add_edge(0, 1);
    // DAGGame::add_edge(0, 2);
    // DAGGame::add_edge(1, 3);
    // DAGGame::add_edge(2, 3);
    // vector<int> pieces = {0}; // 棋子在节点 0
    // int res = DAGGame::solve(pieces);
    // cout << (res ? "First Win" : "Second Win") << endl;

    return 0;
}