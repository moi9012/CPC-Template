struct BIT {
    vector<ll> tr;    // 树状数组核心数组，tr[i] 维护以 i 为右端点、长度为 lowbit(i) 的区间和
    int n;            // 数组规模

    BIT(int _n) : n(_n) { tr.assign(n + 1, 0); }

    /*
     * ================== 树状数组 (Fenwick Tree) ==================
     * 【核心思想】
     * 利用二进制低位（lowbit）的性质，将长度为 n 的数组划分为若干个长度为 2^k 的区间，
     * 构建一棵隐式的树形结构。每个节点 tr[i] 负责维护一段连续区间的和。
     * 
     * 【lowbit 原理】
     * lowbit(x) = x & (-x)：利用补码性质，提取 x 的二进制表示中最低位的 1。
     * 例如 x = 6 (0110)，-x = 1010，x & (-x) = 0010 = 2。
     * 
     * 【操作逻辑】
     * 1. add(x, c)：单点修改。从 x 出发，不断加上 lowbit(x) 向上跳，更新所有包含 x 的区间。
     * 2. query(x)：前缀查询。从 x 出发，不断减去 lowbit(x) 向下跳，累加所有不重叠的区间。
     * 
     * 【时间复杂度】
     * 单点修改 O(log n)，区间查询 O(log n)。
     * 
     * 【常见应用】
     * 1. 求逆序对（离散化 + 树状数组）。
     * 2. 动态前缀和 / 区间和查询。
     * 3. 结合差分实现“区间修改 + 单点查询”。
     * 4. 二维树状数组处理矩阵区间操作。
     * ==============================================================
     */

    // 提取 x 的二进制最低位的 1
    int lowbit(int x) { return x & -x; }

    // 单点修改：在位置 x 加上 c
    void add(int x, ll c) {
        // 从 x 开始，沿着树向上跳，更新所有覆盖 x 的区间
        for (; x <= n; x += lowbit(x)) tr[x] += c;
    }

    // 前缀查询：求 [1, x] 的和
    ll query(int x) {
        ll res = 0;
        // 从 x 开始，沿着树向下跳，累加所有不重叠的区间
        for (; x > 0; x -= lowbit(x)) res += tr[x];
        return res;
    }

    // 区间查询：求 [l, r] 的和（利用前缀和差分）
    ll query(int l, int r) { return query(r) - query(l - 1); }
};

/*
 * ============================================================================================
 * File Description: C++ Template for Competitive Programming (CPC)
 * Template Source: https://github.com/moi9012/CPC-Template
 * Compiler Version: C++17 / C++20
 * Author Notes: Includes common headers, macros, fast I/O, and frequently used data structures.
 *               Suitable for platforms like Codeforces and CodeChef.
 * ============================================================================================
 */