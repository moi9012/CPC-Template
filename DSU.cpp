#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
using namespace std;

/*
 * ================== 1. 基础并查集 (DSU) ==================
 * 【核心思想】
 * 用一棵树表示一个集合，find 找根代表集合归属，union 合并两棵树。
 * 两大优化：
 *   - 路径压缩：find 时把路径上所有点直接挂到根，均摊近似 O(α(n))。
 *   - 按 size 合并：小树挂到大树下，控制树高，避免退化成链。
 * 【时间复杂度】单次操作均摊 O(α(n))，α 为反阿克曼函数， practically 常数级。
 * 【适用】静态/在线合并、只问连通性或集合大小，无需撤销。
 */
struct DSU {
    vector<int> fa, sz;
    int n;
    int components; // 当前连通块数量，方便统计

    DSU(int _n) : n(_n), fa(_n), sz(_n, 1), components(_n) {
        iota(fa.begin(), fa.end(), 0); // fa[i] = i
    }

    // 带路径压缩的 find
    int find(int x) {
        if (fa[x] == x) return x;
        return fa[x] = find(fa[x]); // 压缩：递归回来后直接指向根
    }

    // 合并：把 x 所在集合并入 y 所在集合，成功返回 true
    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;        // 已连通
        if (sz[x] < sz[y]) swap(x, y);   // 按 size 合并：x 是大树
        fa[y] = x;
        sz[x] += sz[y];
        --components;
        return true;
    }

    bool connected(int x, int y) { return find(x) == find(y); }
    int size(int x)  { return sz[find(x)]; }      // x 所在集合元素数
    int count()      { return components; }        // 连通块总数
};

/*
 * ================== 2. 带权并查集 (Weighted DSU) ==================
 * 【核心思想】
 * 在维护 fa 的同时，维护 d[x]：x 到其所在集合根节点的“相对权值”。
 * 关键在 find 时沿路径回溯累加（不能在压缩后直接取，必须先递归再更新 d）。
 * 合并时根据「x 与 y 的关系」反推 y 的根应挂到何处、权值设多少。
 * 【经典应用】食物链（POJ 1182）、区间奇偶、种类判断。
 */
struct WeightedDSU {
    vector<int> fa, d; // d[x]: x 到 fa 路径上根的累计权值（模 3 表示关系时）
    WeightedDSU(int n) : fa(n), d(n, 0) {
        iota(fa.begin(), fa.end(), 0);
    }
    // 带权路径压缩：先递归找根，回溯时累加 d[x]
    int find(int x) {
        if (fa[x] == x) return x;
        int root = find(fa[x]);
        d[x] += d[fa[x]]; // 重要：先更新 d，再改 fa，否则 d[fa[x]] 已被改写
        return fa[x] = root;
    }
    // 合并 x,y 并约束 d[x] 相对 d[y] 增加 w（依题意设计）
    void unite(int x, int y, int w) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return;
        fa[rx] = ry;
        // 使关系成立：d[x] 应等于 d[y]+w，反推根 rx 的偏移
        d[rx] = d[y] + w - d[x];
    }
    // 查询 x 与 y 的相对权值（需已连通）
    int diff(int x, int y) { find(x); find(y); return d[x] - d[y]; }
    bool connected(int x, int y) { return find(x) == find(y); }
};

/*
 * ================== 3. 可撤销并查集 (Rollback DSU) ==================
 * 【核心思想】
 * 支持“撤销上一次合并”，常用于：
 *   - 线段树分治（把边按存在时间段挂到线段树上，DFS 时加边/回溯时撤销）
 *   - 离线动态连通性、Kruskal 重构树相关
 * 【关键约束】不能路径压缩（会改变多个 fa，无法精确回滚），
 *   只能【按 size/rank 合并】来保证树高 O(log n)，find 复杂度 O(log n)。
 *   每次 unite 把“被改动的那个点”压栈，revert 时弹栈恢复。
 */
struct RollbackDSU {
    vector<int> fa, sz, stk;
    int components;
    RollbackDSU(int n) : fa(n), sz(n, 1), components(n) {
        iota(fa.begin(), fa.end(), 0);
    }
    int find(int x) { while (fa[x] != x) x = fa[x]; return x; } // 不压缩！

    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) { stk.push_back(-1); return false; } // 占位，保证栈与操作一一对应
        if (sz[x] < sz[y]) swap(x, y);
        stk.push_back(y);     // 记录被挂到 x 下面的根 y，撤销时恢复 fa[y]=y
        fa[y] = x;
        sz[x] += sz[y];
        --components;
        return true;
    }
    // 撤销最近一次 unite
    void revert() {
        if (stk.empty()) return;
        int y = stk.back(); stk.pop_back();
        if (y == -1) return;          // 对应一次失败的合并，无改动
        int x = find(y);              // y 当前挂在 x 下
        fa[y] = y;
        sz[x] -= sz[y];
        ++components;
    }
    int snapshot() { return stk.size(); } // 取栈深
    void rollback(int snap) {              // 撤销到指定快照
        while (stk.size() > snap) revert();
    }
    bool connected(int x, int y) { return find(x) == find(y); }
};

// ================== 主函数测试 ==================
int main() {
    // 1. 基础 DSU：判断连通与集合大小
    DSU dsu(6);
    dsu.unite(0, 1);
    dsu.unite(1, 2);
    dsu.unite(3, 4);
    cout << "0,2 connected: " << dsu.connected(0, 2) << "\n"; // 1
    cout << "size of {0..2}: " << dsu.size(0) << "\n";        // 3
    cout << "components: " << dsu.count() << "\n";            // 3

    // 2. 可撤销并查集：合并后撤销，回到合并前状态
    RollbackDSU r(4);
    r.unite(0, 1);
    r.unite(1, 2);
    cout << "before rollback, comps: " << r.components << "\n"; // 2
    r.rollback(0); // 撤销全部
    cout << "after rollback, comps: " << r.components << "\n";  // 4

    return 0;
}