// 费马小定理求单个逆元 (p为质数)
ll inv(ll a, ll p) {
    return qpow(a, p - 2, p);
}

// 线性递推求 1~n 的逆元
vector<ll> get_inv(int n, ll p) {
    vector<ll> f(n + 1);
    f[1] = 1;
    for (int i = 2; i <= n; ++i) {
        f[i] = (p - (p / i) * f[p % i] % p) % p;
    }
    return f;
}