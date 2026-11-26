#include <vector>
using namespace std;

vector<int> primes;      // 素数表
vector<bool> is_comp;    // 合数标记数组
vector<int> phi, mu;     // phi[i] 存储欧拉函数值，mu[i] 存储莫比乌斯函数值

/*
 * ================== 线性筛求积性函数 ==================
 * 【核心思想】
 * 利用线性筛的 O(n) 复杂度，结合积性函数的性质递推求解。
 * 对于合数 i * p（p 为其最小质因子），分两种情况讨论：
 * 
 * 【情况 1：i % p != 0（p 与 i 互质）】
 * 由于 p 是质数且 p 不整除 i，根据积性函数的定义：
 * 1. 欧拉函数：phi[i * p] = phi[i] * phi[p] = phi[i] * (p - 1)
 * 2. 莫比乌斯函数：mu[i * p] = mu[i] * mu[p] = mu[i] * (-1) = -mu[i]
 * 
 * 【情况 2：i % p == 0（p 是 i 的最小质因子）】
 * 此时 i 已经包含质因子 p，i * p 相当于在 i 的基础上增加一个 p 的幂次：
 * 1. 欧拉函数：phi[i * p] = phi[i] * p
 *    推导：phi(n) = n * prod(1 - 1/p)。i * p 与 i 的质因子集合完全相同，
 *    只是 n 变成了 i * p，因此 phi[i * p] = (i * p) * prod(...) = p * phi[i]。
 * 2. 莫比乌斯函数：mu[i * p] = 0
 *    推导：i 已经包含质因子 p，再乘一个 p 意味着 i * p 含有平方因子 p^2，
 *    根据 mu 的定义，含有平方因子的数其 mu 值为 0。
 * 
 * 【时间复杂度】
 * 严格 O(n)，每个合数仅被其最小质因子筛掉一次。
 * 
 * 【常见应用】
 * 1. 求 1~n 中与 n 互质的数的和 / 个数。
 * 2. 莫比乌斯反演：求解 gcd 相关的计数问题。
 * 3. 求 1~n 的 phi 和 mu 的前缀和（配合杜教筛处理 n > 10^7 的情况）。
 * ======================================================
 */
void get_num_theory(int n) {
    phi.resize(n + 1);
    mu.resize(n + 1);
    is_comp.assign(n + 1, false);
    
    // 初始化边界条件
    phi[1] = 1; // 1 与 1 互质
    mu[1] = 1;  // 1 没有质因子，(-1)^0 = 1
    
    for (int i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            primes.push_back(i);
            phi[i] = i - 1; // 质数 i 与 1~i-1 都互质
            mu[i] = -1;     // 质数只有一个质因子，(-1)^1 = -1
        }
        
        for (int p : primes) {
            if ((ll)i * p > n) break; // 防止溢出
            is_comp[i * p] = true;    // 标记合数
            
            if (i % p == 0) {
                // p 是 i 的最小质因子
                phi[i * p] = phi[i] * p; // 质因子集合不变，仅 n 乘 p
                mu[i * p] = 0;           // 含有平方因子 p^2
                break; // 核心剪枝：保证每个合数只被最小质因子筛掉
            } else {
                // p 与 i 互质
                phi[i * p] = phi[i] * (p - 1); // 积性函数性质
                mu[i * p] = -mu[i];            // 多一个质因子，符号取反
            }
        }
    }
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