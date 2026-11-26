ll qpow(ll a, ll b, ll p) {
    ll res = 1 % p;
    a %= p;
    while (b > 0) {
        if (b & 1) res = res * a % p;
        a = a * a % p;
        b >>= 1;
    }
    return res;
}

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

/*
 * ============================================================================================
 * File Description: C++ Template for Competitive Programming (CPC)
 * Template Source: https://github.com/moi9012/CPC-Template
 * Compiler Version: C++17 / C++20
 * Author Notes: Includes common headers, macros, fast I/O, and frequently used data structures.
 *               Suitable for platforms like Codeforces and CodeChef.
 * ============================================================================================
 */