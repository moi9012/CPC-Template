#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;

const int NTT_MOD = 998244353; // 常用 NTT 模数，形式为 c * 2^k + 1
const int G = 3;               // 998244353 的原根为 3

/*
 * ================== NTT 快速数论变换 ==================
 * 【核心思想】
 * 与 FFT 类似，NTT 利用单位根的性质将多项式从系数表示法转换为点值表示法，
 * 在点值表示法下多项式乘法为 O(n)，再通过逆变换转回系数表示法。
 * 
 * 【与 FFT 的区别】
 * FFT 使用复数单位根 e^(2πi/n)，存在浮点精度误差；
 * NTT 使用模意义下的 n 次单位根 g^((p-1)/n)，其中 g 是原根，p 是质数。
 * 要求模数 p 满足 p = c * 2^k + 1（如 998244353 = 119 * 2^23 + 1），
 * 以保证存在足够高阶的单位根。
 * 
 * 【时间复杂度】
 * O(n log n)，其中 n 为多项式长度（必须是 2 的幂次）。
 * 
 * 【关键步骤】
 * 1. 位逆序置换：将数组下标按二进制位反转重排，为分治蝴蝶操作做准备。
 * 2. 蝴蝶操作：自底向上合并，每次将长度为 len 的区间拆分为两个 len/2 的子区间，
 *    利用单位根 w 进行蝶形运算。
 * 3. 逆变换：将 w 替换为 w^(-1)（模意义下的逆元），最后整体乘以 n^(-1)。
 * 
 * 【常见应用】
 * 1. 多项式乘法 / 大整数乘法（取模）。
 * 2. 多项式快速幂、多项式求逆、多项式开根（需配合牛顿迭代）。
 * 3. 字符串匹配（带通配符）、集合卷积（子集和/子集卷积）。
 * 4. 生成函数加速递推（如 Catalan 数、划分数的计算）。
 * ======================================================
 */
void ntt(vector<ll> &a, bool inv) {
    int n = a.size();
    
    // 位逆序置换：将下标 i 的二进制位反转后与 j 交换
    // 例如 n=8 时，下标 3(011) 与 6(110) 互换
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit; // 从高位向低位进位
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    
    // 自底向上分治，len 为当前合并的区间长度
    for (int len = 2; len <= n; len <<= 1) {
        // 计算 len 次单位根：w = g^((p-1)/len)
        // 若为逆变换，则取 w 的逆元
        ll w = qpow(G, (NTT_MOD - 1) / len, NTT_MOD);
        if (inv) w = inv(w, NTT_MOD);
        
        // 对每个长度为 len 的区间进行蝴蝶操作
        for (int i = 0; i < n; i += len) {
            ll wn = 1; // wn = w^j，当前使用的单位根幂次
            for (int j = 0; j < len / 2; ++j) {
                // 蝶形运算：u 为左半部分，v 为右半部分乘以单位根
                ll u = a[i + j];
                ll v = a[i + j + len / 2] * wn % NTT_MOD;
                a[i + j] = (u + v) % NTT_MOD;
                a[i + j + len / 2] = (u - v + NTT_MOD) % NTT_MOD;
                wn = wn * w % NTT_MOD; // 更新单位根幂次
            }
        }
    }
    
    // 逆变换最后需要乘以 n 的逆元
    if (inv) {
        ll n_inv = inv(n, NTT_MOD);
        for (auto &x : a) x = x * n_inv % NTT_MOD;
    }
}

/*
 * ================== 多项式乘法 ==================
 * 【核心流程】
 * 1. 计算结果长度 rs = a.size() + b.size() - 1
 * 2. 将 a, b 补齐到 2 的幂次长度 n（NTT 要求长度为 2^k）
 * 3. 分别对 a, b 进行正向 NTT（系数表示 -> 点值表示）
 * 4. 点值相乘（O(n)）
 * 5. 对结果进行逆向 NTT（点值表示 -> 系数表示）
 * 6. 截断到实际长度 rs
 * ================================================
 */
vector<ll> multiply(vector<ll> a, vector<ll> b) {
    int rs = a.size() + b.size() - 1;
    int n = 1;
    while (n < rs) n <<= 1; // 补齐到 2 的幂次
    a.resize(n); b.resize(n);
    
    ntt(a, false); // 正向变换
    ntt(b, false);
    
    // 点值相乘
    for (int (int i = 0; i < n; ++i) a[i] = a[i] * b[i] % NTT_MOD;
    
    ntt(a, true); // 逆向变换
    a.resize(rs); // 截断多余部分
    return a;
}