struct BIT {
    vector<ll> tr;
    int n;
    BIT(int _n) : n(_n) { tr.assign(n + 1, 0); }
    int lowbit(int x) { return x & -x; }
    void add(int x, ll c) {
        for (; x <= n; x += lowbit(x)) tr[x] += c;
    }
    ll query(int x) {
        ll res = 0;
        for (; x > 0; x -= lowbit(x)) res += tr[x];
        return res;
    }
    // 区间 [l, r] 求和
    ll query(int l, int r) { return query(r) - query(l - 1); }
};