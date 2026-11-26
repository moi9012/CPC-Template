struct Block {
    vector<ll> a;       // 原数组，存储每个元素的真实值（不含整块懒标记）
    vector<ll> add;     // 懒标记数组，add[i] 表示第 i 块整体需要加上的值
    vector<ll> sum;     // 块和数组，sum[i] 表示第 i 块内所有元素的和（已包含该块的懒标记贡献）
    int block_size;     // 块的大小，通常取 sqrt(n) 以平衡复杂度

    /*
     * ================== 初始化（build） ==================
     * 【核心思想】
     * 1. 块大小取 sqrt(n)：根据均值不等式，当块长与块数相等时，
     *    单次操作的复杂度 O(块长 + 块数) 达到最小。
     * 2. 预处理每个元素所属的块（隐式通过 i / block_size 计算），
     *    并累加求出每个块的初始区间和。
     * 
     * 【时间复杂度】O(n)
     * =====================================================
     */
    Block(vector<ll> &arr) : a(arr) {
        int n = a.size();
        block_size = max(1, (int)sqrt(n)); // 防止 n=0 时 sqrt(n)=0 导致除零错误
        add.assign(n, 0);
        sum.assign(n, 0);
        for (int i = 0; i < n; ++i) {
            sum[i / block_size] += a[i]; // 累加求每个块的初始和
        }
    }

    /*
     * ================== 区间修改（update） ==================
     * 【核心思想】
     * 将区间 [l, r] 的修改拆分为三部分：
     * 1. 左碎块（不完整块）：暴力修改 a[i]，并更新所在块的 sum。
     * 2. 右碎块（不完整块）：暴力修改 a[i]，并更新所在块的 sum。
     * 3. 中间整块：只修改懒标记 add[i]，并更新 sum[i] += c * block_size。
     * 
     * 【时间复杂度】O(√n)
     * ======================================================
     */
    void update(int l, int r, ll c) {
        int bl = l / block_size, br = r / block_size;
        
        if (bl == br) { // l 和 r 在同一个块内，直接暴力修改
            for (int i = l; i <= r; ++i) a[i] += c;
            sum[bl] += c * (r - l + 1); // 更新该块的区间和
        } else {
            // 1. 处理左碎块：从 l 到左端点所在块的末尾
            for (int i = l; i < (bl + 1) * block_size; ++i) a[i] += c;
            sum[bl] += c * ((bl + 1) * block_size - l);
            
            // 2. 处理右碎块：从右端点所在块的开头到 r
            for (int i = br * block_size; i <= r; ++i) a[i] += c;
            sum[br] += c * (r - br * block_size + 1);
            
            // 3. 处理中间整块：只打懒标记，并同步更新块和
            for (int i = bl + 1; i < br; ++i) {
                add[i] += c;
                sum[i] += c * block_size; // 整块长度固定为 block_size
            }
        }
    }

    /*
     * ================== 区间查询（query） ==================
     * 【核心思想】
     * 查询区间 [l, r] 的和，同样拆分为三部分：
     * 1. 左碎块：暴力累加 a[i] + add[bl]（真实值 = 存储值 + 懒标记）。
     * 2. 右碎块：暴力累加 a[i] + add[br]。
     * 3. 中间整块：直接累加预处理的 sum[i]（已包含懒标记贡献）。
     * 
     * 【时间复杂度】O(√n)
     * ======================================================
     */
    ll query(int l, int r) {
        int bl = l / block_size, br = r / block_size;
        ll res = 0;
        
        if (bl == br) { // l 和 r 在同一个块内，直接暴力查询
            for (int i = l; i <= r; ++i) res += a[i] + add[bl];
        } else {
            // 1. 处理左碎块
            for (int i = l; i < (bl + 1) * block_size; ++i) res += a[i] + add[bl];
            
            // 2. 处理右碎块
            for (int i = br * block_size; i <= r; ++i) res += a[i] + add[br];
            
            // 3. 处理中间整块：直接加块和
            for (int i = bl + 1; i < br; ++i) res += sum[i];
        }
        
        return res;
    }
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