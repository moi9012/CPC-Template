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
#include <functional>   // std::function

using namespace std;

using ll  = long long;
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

[[maybe_unused]] const int MOD  = 1e9 + 7;
[[maybe_unused]] const int INF  = 0x3f3f3f3f;
[[maybe_unused]] const ll  LINF = 1e18;
[[maybe_unused]] const int MAXN = 200005;   // 常用数组 / bitset 大小上限

void solve() {
    // Write the logic for a single test case here
    int n;
    if (!(cin >> n)) return;

    // Example logic
    cout << n * 2 << endl;
}

/*
 * ============================================================================================
 * 模板补充：竞赛常见写法速查（放在 solve 下面，按需复制到具体题目里改写）
 * 说明：以下为带编号的速查片段，以注释形式保留，以免与主程序变量冲突；
 *       真正写题时把对应片段粘贴到 solve() 内部即可，注意多组数据要重置容器与数组。
 * ============================================================================================
 */
/*
 * ---------------------------- 1. 常见变量 / 结构读入 ----------------------------
 *
 * (1) 读入一个 n 和数组 a（下标从 1 开始，配合 rep 宏最方便）
 *     int n; cin >> n;
 *     vector<int> a(n + 1);
 *     for (int i = 1; i <= n; ++i) cin >> a[i];
 *
 *     // 也可一行搞定：
 *     // vector<int> a(n); for (int &x : a) cin >> x;
 *
 * (2) 读入一个 n 点 m 边的有向图（邻接表）；无向图把下面两行都加上
 *     int n, m; cin >> n >> m;
 *     vector<vector<int>> g(n + 1);
 *     for (int i = 0; i < m; ++i) {
 *         int u, v; cin >> u >> v;
 *         g[u].pb(v);          // 有向：u -> v
 *         // g[v].pb(u);       // 无向图再补这一行
 *     }
 *
 * (3) 读入带权图（邻接表存 (to, w)）
 *     int n, m; cin >> n >> m;
 *     vector<vector<pii>> gw(n + 1);
 *     for (int i = 0; i < m; ++i) {
 *         int u, v, w; cin >> u >> v >> w;
 *         gw[u].pb({v, w});
 *         // gw[v].pb({u, w}); // 无向
 *     }
 *
 * (4) 读入二维网格 / 矩阵
 *     int n, m; cin >> n >> m;
 *     vector<string> grid(n);
 *     for (int i = 0; i < n; ++i) cin >> grid[i];
 *     // 整数矩阵：vector<vector<int>> mat(n, vector<int>(m)); 双重循环 cin
 *
 * (5) 读到 EOF（不确定组数）：while (cin >> x) { ... }
 *     读到一行（含空格）：string line; getline(cin, line);
 *
 * ---------------------------- 2. std::function 写 DFS（多组数据友好） ----------------------------
 *
 *     把 visited / dist / 父节点等状态放在 solve 局部，每组数据自动重建，天然不残留；
 *     dfs 用 function 递归捕获这些局部变量。
 *
 *     vector<vector<int>> g(n + 1);
 *     // ... 建图（见 1.(2)） ...
 *
 *     vector<int> vis(n + 1, 0);          // 每解一次 solve 都会重新清零，无需手动 memset
 *     auto dfs = [&](auto &&self, int u, int fa) -> void {
 *         vis[u] = 1;
 *         for (int v : g[u]) {
 *             if (v == fa) continue;       // 树形：跳过父亲；普通图判图用 if(vis[v]) continue;
 *             self(self, v, u);            // 泛型 lambda(Y-combinator) 写法，比 std::function 快
 *         }
 *     };
 *     dfs(dfs, 1, 0);
 *
 *     // 经典 std::function 写法（可读性最好，稍有开销，普通题够用）：
 *     function<void(int,int)> dfs2 = [&](int u, int fa) {
 *         vis[u] = 1;
 *         for (int v : g[u]) if (v != fa) dfs2(v, u);
 *     };
 *
 * ---------------------------- 3. 自定义比较：排序 / 优先队列 ----------------------------
 *
 * (1) 排序 sort（默认升序）
 *     vector<int> v; sort(all(v));                 // 升序
 *     sort(all(v), greater<int>());                // 降序
 *
 *     // pair 自定义：先按第一关键字升序，第一相同按第二关键字降序
 *     vector<pii> p;
 *     sort(all(p), [](const pii &a, const pii &b){
 *         if (a.fi != b.fi) return a.fi < b.fi;
 *         return a.se > b.se;
 *     });
 *
 *     // 结构体自定义排序：可写 lambda（如上），或重载 < / 定义 cmp
 *     struct Node { int id, x, y; };
 *     sort(all(nd), [](const Node &a, const Node &b){
 *         if (a.x != b.x) return a.x < b.x;
 *         return a.y < b.y;
 *     });
 *
 * (2) 优先队列 priority_queue（默认是【大顶堆】！要小顶需显式给比较）
 *     priority_queue<int> maxHeap;                                  // 大顶：top 最大
 *     priority_queue<int, vector<int>, greater<int>> minHeap;       // 小顶：top 最小
 *
 *     // pair 小顶（按 first 再 second 升序）：
 *     priority_queue<pii, vector<pii>, greater<pii>> pqP;
 *
 *     // 自定义结构体小顶：cmp 返回 true 表示 a 优先级【低于】b（与 sort 比较器方向相反，易错！）
 *     struct Cmp {
 *         bool operator()(const Node &a, const Node &b) const {
 *             return a.x > b.x;     // x 越小越先出队
 *         }
 *     };
 *     priority_queue<Node, vector<Node>, Cmp> pqNode;
 *     // 用法：pqNode.push(node); Node t = pqNode.top(); pqNode.pop();
 *
 * ---------------------------- 4. map / set 查找键值 ----------------------------
 *
 * (1) map<K, V>：计数 / 映射
 *     map<int,int> cnt;             // 有序，O(logn)；unordered_map 平均 O(1)
 *     cnt[x]++;                     // 不存在会先插入默认值 0
 *     if (cnt.count(x)) { ... }     // 判断 key 是否存在（不改容器）
 *     auto it = cnt.find(x);        // 查找返回迭代器
 *     if (it != cnt.end()) { int k = it->fi, v = it->se; }
 *     cnt.erase(x);                 // 删除某 key
 *
 * (2) set<T>：去重 / 有序集合
 *     set<int> s;
 *     s.insert(x);
 *     if (s.count(x)) { ... }       // 判断是否存在
 *     auto it = s.lower_bound(x);   // >= x 的第一个
 *     auto jt = s.upper_bound(x);   // >  x 的第一个
 *     s.erase(it); s.erase(x);
 *
 *     // set 的元素即 key，不能直接改：先删再插
 *     // s.erase(oldVal); s.insert(newVal);
 *
 *     // 找最接近 x 的元素（利用 lower_bound）
 *     auto p = s.lower_bound(x);
 *     // p 是 >=x 最小；若 p!=begin 则 prev(p) 是 <x 最大，二者比较即为最近
 *
 * ---------------------------- 5. bitset 初始化 / 修改 / 计数 ----------------------------
 *
 *     bitset<MAXN> bs;              // 编译期定长，栈上分配，比 vector<bool> 快且省内存
 *
 *     bs.reset();                   // 全部置 0
 *     bs.set();                     // 全部置 1
 *     bs.set(pos);                  // 把第 pos 位置 1
 *     bs.set(pos, 0);               // 把第 pos 位置 0（val 为 bool）
 *     bs.flip();                    // 全部取反
 *     bs.flip(pos);                 // 翻转第 pos 位
 *
 *     if (bs.test(pos)) { ... }     // 查询某位是否为 1（等价 bs[pos]）
 *     bool b = bs[pos];
 *     bs[pos] = 1;                  // 直接赋值某位
 *
 *     int c = bs.count();           // 统计 1 的个数
 *     bool any = bs.any();          // 是否有 1
 *     bool none = bs.none();        // 是否全 0
 *     bool allz = bs.all();         // 是否全 1（部分编译器/版本可用）
 *
 *     // 位运算与整型互转
 *     bitset<MAXN> t = bs & bs2;    // 与/或/异或：bs & bs2 / bs | bs2 / bs ^ bs2
 *     ll num = bs.to_ullong();      // 转无符号整数（位数不超 64 时安全）
 *     string str = bs.to_string();  // 转 '0'/'1' 字符串
 *
 *     // 从字符串 / 整数初始化
 *     bitset<MAXN> fromStr(string("10110"));
 *     bitset<MAXN> fromInt(123);
 */

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // If the problem has multiple test cases, uncomment the line below
    // cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
