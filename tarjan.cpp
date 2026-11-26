struct TarjanSCC {
    vector<vector<int>> adj;  // 邻接表存储有向图
    vector<int> dfn;          // dfn[u]: 时间戳，节点 u 是 DFS 中第几个被访问到的
    vector<int> low;          // low[u]: 追溯值，u 或其子树能追溯到的、且在栈中的最早节点的时间戳
    vector<int> stk;          // 栈，存储当前 DFS 路径上尚未归属任何 SCC 的节点
    vector<int> comp;         // comp[u]: 记录节点 u 属于第几个强连通分量（缩点后的新编号）
    vector<bool> in_stk;      // in_stk[u]: 标记节点 u 是否在栈中（用于判断是否构成环）
    int timer;                // 全局时间戳计数器
    int scc_cnt;              // 强连通分量计数器

    TarjanSCC(int n) : adj(n), dfn(n, 0), low(n, 0), comp(n, -1), in_stk(n, false), timer(0), scc_cnt(0) {}
    
    void add_edge(int u, int v) { adj[u].push_back(v); }

    /*
     * ================== Tarjan 核心 DFS ==================
     * 【核心思想】
     * 1. 对每个节点 u，记录 dfn[u] 和 low[u]，并将 u 入栈。
     * 2. 遍历 u 的邻接点 v：
     *    - 若 v 未访问（树边）：递归 DFS(v)，回溯后 low[u] = min(low[u], low[v])。
     *    - 若 v 已访问且在栈中（回边/交叉边）：说明存在环，low[u] = min(low[u], dfn[v])。
     * 3. 当 dfn[u] == low[u] 时，u 是当前 SCC 的“根节点”，
     *    从栈顶弹出节点直到 u，这些节点构成一个完整的 SCC。
     * 
     * 【时间复杂度】O(n + m)
     * =====================================================
     */
    void dfs(int u) {
        dfn[u] = low[u] = ++timer; // 记录时间戳
        stk.push_back(u);          // 入栈
        in_stk[u] = true;          // 标记在栈中

        for (int v : adj[u]) {
            if (!dfn[v]) {         // 情况1：v 未访问（树边），递归
                dfs(v);
                low[u] = min(low[u], low[v]); // 回溯时用子节点的 low 更新
            } else if (in_stk[v]) { // 情况2：v 已访问且在栈中（回边/交叉边），存在环
                low[u] = min(low[u], dfn[v]); // 注意：这里用 dfn[v] 而非 low[v]
            }
            // 情况3：v 已访问且不在栈中（已归属某个 SCC），忽略
        }

        // 找到 SCC 的根节点：dfn[u] == low[u]
        if (dfn[u] == low[u]) {
            ++scc_cnt; // 新增一个 SCC
            while (true) {
                int v = stk.back(); stk.pop_back();
                in_stk[v] = false; // 出栈，标记不在栈中
                comp[v] = scc_cnt; // 记录 v 属于当前 SCC
                if (v == u) break; // 弹到根节点 u 为止
            }
        }
    }

    /*
     * ================== 主函数：遍历所有节点 ==================
     * 【注意】图可能不连通，需对每个未访问节点调用 dfs
     * ==========================================================
     */
    void run() {
        for (int i = 0; i < adj.size(); ++i) {
            if (!dfn[i]) dfs(i);
        }
    }
};