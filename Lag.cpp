// 给定 n 个点 (x[i], y[i])，求 x 处的值，复杂度 O(n)
ll lagrange(vector<ll> &x, vector<ll> &y, ll X, ll p) {
    int n = x.size();
    ll res = 0;
    for (int i = 0; i < n; ++i) {
        ll num = y[i], den = 1;
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            num = num * ((X - x[j]) % p + p) % p;
            den = den * ((x[i] - x[j]) % p + p) % p;
        }
        res = (res + num * inv(den, p)) % p;
    }
    return res;
}