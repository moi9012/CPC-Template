struct TarjanSCC {
    vector<vector<int>> adj;
    vector<int> dfn, low, stk, comp;
    vector<bool> in_stk;
    int timer, scc_cnt;
    TarjanSCC(int n) : adj(n), dfn(n, 0), low(n, 0), comp(n, -1), in_stk(n, false), timer(0), scc_cnt(0) {}
    void add_edge(int u, int v) { adj[u].push_back(v); }
    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        stk.push_back(u);
        in_stk[u] = true;
        for (int v : adj[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stk[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            ++scc_cnt;
            while (true) {
                int v = stk.back(); stk.pop_back();
                in_stk[v] = false;
                comp[v] = scc_cnt;
                if (v == u) break;
            }
        }
    }
    void run() {
        for (int i = 0; i < adj.size(); ++i) {
            if (!dfn[i]) dfs(i);
        }
    }
};