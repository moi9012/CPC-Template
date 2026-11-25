struct STTable {
    vector<vector<int>> f;
    vector<int> log;
    void build(vector<int> &a) {
        int n = a.size();
        log.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) log[i] = log[i >> 1] + 1;
        f.assign(log[n] + 1, vector<int>(n));
        f[0] = a;
        for (int j = 1; (1 << j) <= n; ++j) {
            for (int i = 0; i + (1 << j) <= n; ++i) {
                f[j][i] = max(f[j - 1][i], f[j - 1][i + (1 << (j - 1))]);
            }
        }
    }
    int query(int l, int r) {
        int k = log[r - l + 1];
        return max(f[k][l], f[k][r - (1 << k) + 1]);
    }
};	