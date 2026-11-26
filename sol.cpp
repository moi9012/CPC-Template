/*
 * ============================================================================================
 * File Description: C++ Template for Competitive Programming (CPC)
 * Template Source: https://github.com/moi9012/CPC-Template
 * Compiler Version: C++17 / C++20
 * Author Notes: Includes common headers, macros, fast I/O, and frequently used data structures.
 *               Suitable for platforms like Codeforces and CodeChef.
 * ============================================================================================
 */
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <string>
#include <climits>
#include <utility>
#include <bitset>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

#define rep(i, a, b) for (int i = (a); i <= (b); ++i)
#define per(i, a, b) for (int i = (a); i >= (b); --i)
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define all(x) x.begin(), x.end()

const int MOD = 1e9 + 7;
const int INF = 0x3f3f3f3f;
const ll LINF = 1e18;

ll gcd(ll a, ll b) {
    return b ? gcd(b, a % b) : a; 
}

struct SegmentTree {
    struct Node {
        int l, r;
        ll sum, add, gcdd, minn, mincnt, ans;
    } tr[400005];

    void push_up(int u) {
        tr[u].sum = tr[u << 1].sum + tr[u << 1 | 1].sum;
        tr[u].gcdd = gcd(tr[u << 1].gcdd, tr[u << 1 | 1].gcdd);
        tr[u].minn = min(tr[u << 1].minn, tr[u << 1 | 1].minn);
        if (tr[u << 1].minn < tr[u << 1 | 1].minn) {
			tr[u].minn = tr[u << 1].minn;
			tr[u].mincnt = tr[u << 1].mincnt;
		} else if (tr[u << 1].minn > tr[u << 1 | 1].minn) {
			tr[u].minn = tr[u << 1 | 1].minn;
			tr[u].mincnt = tr[u << 1 | 1].mincnt;
		} else {
			tr[u].minn = tr[u << 1].minn;
			tr[u].mincnt = tr[u << 1].mincnt + tr[u << 1 | 1].mincnt;
		}
        tr[u].ans = tr[u << 1].ans + tr[u << 1 | 1].ans;
    }
    
    void build(int u, int l, int r, vector<ll> &a) {
    	
        tr[u] = {l, r, a[l], 0, 0, INF, 1, 0};
        // cout << a[l] << '\n';
        if (l == r) return;
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        push_up(u);
    }
    
    ll query(int u, int l, int r) {
        if (tr[u].l >= l && tr[u].r <= r) return tr[u].ans;
       
        int mid = (tr[u].l + tr[u].r) >> 1;
        ll res = 0;
        if (l <= mid) res += query(u << 1, l, r);
        if (r > mid) res += query(u << 1 | 1, l, r);
        return res;
    }
};


int main() {
    ios::sync_with_stdio(false);
    
    cin.tie(nullptr);
    cout.tie(nullptr);
    
   	SegmentTree seg;
   	int n;
   	cin >> n;
   	vector<ll> a(n + 1);
   	
   	for (int i = 1; i <= n; i++) cin >> a[i];
   	seg.build(1, 1, n, a);
   	
   	cout << seg.query(1, 1, n) << '\n';
    
    return 0;
}