#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;

const ll NTT_MOD = 998244353;
const ll G = 3;

// 快速幂
ll qpow(ll a, ll b, ll mod) {
    ll r = 1;
    a %= mod;
    while (b > 0) {
        if (b & 1) r = r * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return r;
}

// 求逆元（费马小定理）
ll inv(ll a, ll mod = NTT_MOD) {
    return qpow(a, mod - 2, mod);
}

void ntt(vector<ll> &a, bool inverse) {   // ← 参数改名为 inverse
    int n = a.size();

    // 位逆序置换
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }

    // 自底向上分治
    for (int len = 2; len <= n; len <<= 1) {
        ll w = qpow(G, (NTT_MOD - 1) / len, NTT_MOD);
        if (inverse) w = inv(w);          // ← 用 inverse，调用 inv 函数

        for (int i = 0; i < n; i += len) {
            ll wn = 1;
            for (int j = 0; j < len / 2; ++j) {
                ll u = a[i + j];
                ll v = a[i + j + len / 2] * wn % NTT_MOD;
                a[i + j] = (u + v) % NTT_MOD;
                a[i + j + len / 2] = (u - v + NTT_MOD) % NTT_MOD;
                wn = wn * w % NTT_MOD;
            }
        }
    }

    // 逆变换乘 n 的逆元
    if (inverse) {
        ll n_inv = inv(n);                // ← 同理
        for (auto &x : a) x = x * n_inv % NTT_MOD;
    }
}

// 多项式乘法
vector<ll> multiply(vector<ll> a, vector<ll> b) {
    int rs = a.size() + b.size() - 1;
    int n = 1;
    while (n < rs) n <<= 1;
    a.resize(n);
    b.resize(n);

    ntt(a, false);
    ntt(b, false);

    for (int i = 0; i < n; ++i)          // ← 修正括号
        a[i] = a[i] * b[i] % NTT_MOD;

    ntt(a, true);
    a.resize(rs);
    return a;
}

int main() {
	int n, m;
	cin >> n >> m;
	vector<ll> a(n), b(m);
	for (int i = 0; i < n; i++) cin >> a[i];
	for (int i = 0; i < m; i++) cin >> b[i];
	vector<ll> c = multiply(a, b);
	for (auto i : c) cout << i << ' ';
	cout << '\n';
}