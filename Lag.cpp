/*
 * ================== 拉格朗日插值 ==================
 * 【核心思想】
 * 给定 n 个点 (x_i, y_i)，构造 n 个基函数 F_i(x)，使得：
 *   F_i(x_i) = y_i
 *   F_i(x_j) = 0 (j ≠ i)
 * 则最终多项式 F(x) = Σ F_i(x) 必然经过所有给定点。
 * 
 * 【基函数构造】
 * F_i(x) = y_i * ∏_{j≠i} (x - x_j) / (x_i - x_j)
 * 当 x = x_i 时，分子分母相等，F_i(x_i) = y_i；
 * 当 x = x_j (j≠i) 时，分子含 (x_j - x_j) = 0，F_i(x_j) = 0。
 * 
 * 【模意义下的除法】
 * 模 p 下的除法转化为乘以逆元：a / b ≡ a * b^(-1) (mod p)
 * 使用费马小定理求逆元：b^(-1) ≡ b^(p-2) (mod p)，p 为质数。
 * 
 * 【时间复杂度】O(n^2)，其中 n 为点的数量
 * 
 * 【常见应用】
 * 1. 自然数幂和：∑_{i=1}^n i^k 是关于 n 的 k+1 次多项式，
 *    预处理前 k+2 项后插值即可 O(k) 求解。
 * 2. 组合计数：某些组合数求和可转化为多项式求值。
 * 3. 数列递推：当递推式复杂但前几项易求时，可猜测多项式次数后插值。
 * ==================================================
 */
ll lagrange(vector<ll> &x, vector<ll> &y, ll X, ll p) {
    int n = x.size();
    ll res = 0;
    
    // 对每个点 i，计算其基函数 F_i(X) 并累加
    for (int i = 0; i < n; ++i) {
        ll num = y[i], den = 1; // num 为分子，den 为分母
        
        // 计算基函数：y_i * ∏_{j≠i} (X - x_j) / (x_i - x_j)
        for (int j = 0; j < n; ++j) {
            if (i == j) continue; // 跳过自身
            // 分子：累乘 (X - x_j)，注意取模防负数
            num = num * ((X - x[j]) % p + p) % p;
            // 分母：累乘 (x_i - x_j)，注意取模防负数
            den = den * ((x[i] - x[j]) % p + p) % p;
        }
        
        // 累加：res += num / den = num * den^(-1)
        res = (res + num * inv(den, p)) % p;
    }
    
    return res;
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