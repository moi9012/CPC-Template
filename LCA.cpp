struct LCA {
    vector<vector<int>> adj;
    vector<vector<int>> fa;
    vector<int> dep;
    int LOG;
    LCA(int n) : adj(n), dep(n), LOG(1) {
        while ((1 << LOG) < n) LOG++;
        fa.assign(LOG, vector<int>(n, -1));
    }
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    void dfs(int u, int parent) {
        fa[0][u] = parent;
        for (int v : adj[u]) {
            if (v == parent) continue;
            dep[v] = dep[u] + 1;
            dfs(v, u);
        }
    }
    void init(int root) {
        dfs(root, -1);
        for (int k = 1; k < LOG; ++k) {
            for (int i = 0; i < adj.size(); ++i) {
                if (fa[k - 1][i] != -1)
                    fa[k][i] = fa[k - 1][fa[k - 1][i]];
            }
        }
    }
    int query(int u, int v) {
        if (dep[u] < dep[v]) swap(u, v);
        int diff = dep[u] - dep[v];
        for (int k = 0; k < LOG; ++k) {
            if ((diff >> k) & 1) u = fa[k][u];
        }
        if (u == v) return u;
        for (int k = LOG - 1; k >= 0; --k) {
            if (fa[k][u] != fa[k][v]) {
                u = fa[k][u];
                v = fa[k][v];
            }
        }
        return fa[0][u];
    }
};