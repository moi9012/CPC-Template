#include <iostream>
using namespace std;
typedef long long ll;

// 1. 欧几里得算法 (GCD)
ll gcd(ll a, ll b) {
    return b == 0 ? a : gcd(b, a % b);
}

// 2. 扩展欧几里得算法 (exgcd)
// 求解 ax + by = gcd(a, b)，返回 gcd(a, b)，并更新 x, y
ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1; y = 0;
        return a;
    }
    ll d = exgcd(b, a % b, y, x); // 注意这里直接交换了 x 和 y 的引用
    y -= (a / b) * x; // 回溯时更新 y
    return d;
}

// 3. 类欧几里得算法 (基础版：求 sum_{i=0}^{n} floor((a*i+b)/c))
// 注意：此模板假设 a, b, c, n >= 0
ll floor_sum(ll n, ll a, ll b, ll c) {
    if (n < 0) return 0; // 边界保护
    
    ll ans = 0;
    // 降维取模：提取 a 和 b 中的整数部分
    if (a >= c) {
        ans += (n * (n + 1) / 2) * (a / c);
        a %= c;
    }
    if (b >= c) {
        ans += (n + 1) * (b / c);
        b %= c;
    }
    
    // 交换坐标轴：此时 a < c, b < c
    ll m = (a * n + b) / c; // 最大高度
    if (m == 0) return ans; // 高度为0，没有整点
    
    // 递归求解，注意参数变化：(n, a, b, c) -> (m-1, c, c-b-1, a)
    ans += m * n - floor_sum(m - 1, c, c - b - 1, a);
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 测试 GCD
    cout << "gcd(12, 18) = " << gcd(12, 18) << "\n";

    // 测试 exgcd
    ll x, y;
    ll d = exgcd(12, 18, x, y);
    cout << "exgcd(12, 18): gcd=" << d << ", x=" << x << ", y=" << y << "\n";

    // 测试 类欧 (求 sum_{i=0}^{4} floor((2*i+3)/5))
    // i=0: 0, i=1: 1, i=2: 1, i=3: 2, i=4: 2. 总和 = 6
    cout << "floor_sum(n=4, a=2, b=3, c=5) = " << floor_sum(4, 2, 3, 5) << "\n";

    return 0;
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