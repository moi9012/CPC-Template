#include <vector>
#include <algorithm>
using namespace std;

struct LCA {
    vector<vector<int>> adj;  // 树的邻接表
    vector<vector<int>> fa;   // fa[k][i] 表示节点 i 向上跳 2^k 步到达的祖先
    vector<int> dep;          // dep[i] 表示节点 i 的深度（根节点深度为 0）
    int LOG;                  // 倍增的最大层级，满足 2^LOG >= n

    LCA(int n) : adj(n), dep(n), LOG(1) {
        // 计算 LOG 的大小，确保 2^LOG >= n，防止跳跃时越界
        while ((1 << LOG) < n) LOG++;
        fa.assign(LOG, vector<int>(n, -1)); // -1 表示不存在该祖先
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 第一次 DFS：预处理每个节点的直接父亲（fa[0]）和深度（dep）
    void dfs(int u, int parent) {
        fa[0][u] = parent;
        for (int v : adj[u]) {
            if (v == parent) continue; // 防止向父节点递归
            dep[v] = dep[u] + 1;
            dfs(v, u);
        }
    }

    /*
     * ================== LCA 倍增法预处理 ==================
     * 【核心思想】
     * 利用动态规划的思想，每个节点向上跳 2^k 步，等价于先跳 2^(k-1) 步，
     * 再从那个节点继续跳 2^(k-1) 步。即：fa[k][i] = fa[k-1][fa[k-1][i]]。
     * 
     * 【时间复杂度】
     * 预处理 O(n log n)，空间复杂度 O(n log n)。
     * =======================================================
     */
    void init(int root) {
        dfs(root, -1); // 从根节点开始 DFS，根节点的父节点设为 -1
        
        // 构建倍增表
        for (int k = 1; k < LOG; ++k) {
            for (int i = 0; i < (int)adj.size(); ++i) {
                // 如果 2^(k-1) 步的祖先存在，才能继续跳 2^(k-1) 步
                if (fa[k - 1][i] != -1)
                    fa[k][i] = fa[k - 1][fa[k - 1][i]];
            }
        }
    }

    /*
     * ================== LCA 倍增法查询 ==================
     * 【核心思想】
     * 1. 深度对齐：假设 u 比 v 深，先将 u 向上跳 (dep[u] - dep[v]) 步，使两者深度相同。
     *    利用二进制拆分，将跳跃步数拆分为若干个 2^k 的和。
     * 2. 同步上跳：如果对齐后 u == v，说明 v 就是 LCA；否则，两者从大到小枚举 k，
     *    只要 fa[k][u] != fa[k][v]，就同时向上跳 2^k 步。
     * 3. 最终结果：循环结束时，u 和 v 会停在 LCA 的直接子节点上，返回 fa[0][u] 即可。
     * 
     * 【时间复杂度】
     * 单次查询 O(log n)。
     * 
     * 【常见应用】
     * 1. 树上两点距离：dist(u, v) = dep[u] + dep[v] - 2 * dep[lca]。
     * 2. 树上差分：利用 LCA 实现 O(log n) 的路径修改（如路径加权、路径染色）。
     * 3. 求树上第 k 个祖先 / 路径中点。
     * =====================================================
     */
    int query(int u, int v) {
        // 保证 u 是较深的节点
        if (dep[u] < dep[v]) swap(u, v);
        
        // 第一步：将 u 向上跳，使其与 v 深度相同
        int diff = dep[u] - dep[v];
        for (int k = 0; k < LOG; ++k) {
            // 二进制拆分：如果 diff 的第 k 位为 1，则跳 2^k 步
            if ((diff >> k) & 1) u = fa[k][u];
        }
        
        // 如果对齐后重合，说明 v 就是 LCA
        if (u == v) return u;
        
        // 第二步：u 和 v 同步向上跳，寻找 LCA
        // 注意：必须从大到小枚举 k，保证贪心地尽可能多地跳
        for (int k = LOG - 1; k >= 0; --k) {
            // 只有当 2^k 步的祖先不同时才跳，确保最终停在 LCA 的下方
            if (fa[k][u] != fa[k][v]) {
                u = fa[k][u];
                v = fa[k][v];
            }
        }
        
        // 此时 u 和 v 是 LCA 的直接子节点，返回其父节点即为 LCA
        return fa[0][u];
    }
};