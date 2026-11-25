const int NTT_MOD = 998244353; // 原根为 3
const int G = 3;

void ntt(vector<ll> &a, bool inv) {
    int n = a.size();
    // 位逆序置换
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        ll w = qpow(G, (NTT_MOD - 1) / len, NTT_MOD);
        if (inv) w = inv(w, NTT_MOD);
        for (int i = 0; i < n; i += len) {
            ll wn = 1;
            for (int j = 0; j < len / 2; ++j) {
                ll u = a[i + j], v = a[i + j + len / 2] * wn % NTT_MOD;
                a[i + j] = (u + v) % NTT_MOD;
                a[i + j + len / 2] = (u - v + NTT_MOD) % NTT_MOD;
                wn = wn * w % NTT_MOD;
            }
        }
    }
    if (inv) {
        ll n_inv = inv(n, NTT_MOD);
        for (auto &x : a) x = x * n_inv % NTT_MOD;
    }
}

vector<ll> multiply(vector<ll> a, vector<ll> b) {
    int rs = a.size() + b.size() - 1;
    int n = 1;
    while (n < rs) n <<= 1;
    a.resize(n); b.resize(n);
    ntt(a, false); ntt(b, false);
    for (int i = 0; i < n; ++i) a[i] = a[i] * b[i] % NTT_MOD;
    ntt(a, true);
    a.resize(rs);
    return a;
}