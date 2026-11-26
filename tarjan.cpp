#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/*
 * ================== Tarjan 求强连通分量 (SCC) ==================
 * 【核心思想】
 * 1. 对每个节点 u，记录 dfn[u] 和 low[u]，并将 u 入栈。
 * 2. 遍历 u 的邻接点 v：
 *    - 若 v 未访问（树边）：递归 DFS(v)，回溯后 low[u] = min(low[u], low[v])。
 *    - 若 v 已访问且在栈中（回边/交叉边）：说明存在环，low[u] = min(low[u], dfn[v])。
 * 3. 当 dfn[u] == low[u] 时，u 是当前 SCC 的“根节点”，
 *    从栈顶弹出节点直到 u，这些节点构成一个完整的 SCC。
 * 
 * 【时间复杂度】O(n + m)
 * ===============================================================
 */
struct TarjanSCC {
    vector<vector<int>> adj;  // 邻接表存储有向图
    vector<int> dfn;          // dfn[u]: 时间戳，节点 u 是 DFS 中第几个被访问到的
    vector<int> low;          // low[u]: 追溯值，u 或其子树能追溯到的、且在栈中的最早节点的时间戳
    vector<int> stk;          // 栈，存储当前 DFS 路径上尚未归属任何 SCC 的节点
    vector<int> comp;         // comp[u]: 记录节点 u 属于第几个强连通分量（缩点后的新编号）
    vector<bool> in_stk;      // in_stk[u]: 标记节点 u 是否在栈中（用于判断是否构成环）
    int timer;                // 全局时间戳计数器
    int scc_cnt;              // 强连通分量计数器

    TarjanSCC(int n) : adj(n), dfn(n, 0), low(n, 0), comp(n, -1), in_stk(n, false), timer(0), scc_cnt(0) {}
    
    void add_edge(int u, int v) { adj[u].push_back(v); }

    void dfs(int u) {
        dfn[u] = low[u] = ++timer; // 记录时间戳
        stk.push_back(u);          // 入栈
        in_stk[u] = true;          // 标记在栈中

        for (int v : adj[u]) {
            if (!dfn[v]) {         // 情况1：v 未访问（树边），递归
                dfs(v);
                low[u] = min(low[u], low[v]); // 回溯时用子节点的 low 更新
            } else if (in_stk[v]) { // 情况2：v 已访问且在栈中（回边/交叉边），存在环
                low[u] = min(low[u], dfn[v]); // 注意：这里用 dfn[v] 而非 low[v]
            }
            // 情况3：v 已访问且不在栈中（已归属某个 SCC），忽略
        }

        // 找到 SCC 的根节点：dfn[u] == low[u]
        if (dfn[u] == low[u]) {
            ++scc_cnt; // 新增一个 SCC
            while (true) {
                int v = stk.back(); stk.pop_back();
                in_stk[v] = false; // 出栈，标记不在栈中
                comp[v] = scc_cnt; // 记录 v 属于当前 SCC
                if (v == u) break; // 弹到根节点 u 为止
            }
        }
    }

    void run() {
        for (int i = 0; i < adj.size(); ++i) {
            if (!dfn[i]) dfs(i);
        }
    }
};

/*
 * ================== 2-SAT 求解器 ==================
 * 【变量编号约定】
 * - 变量 x_i (0-indexed) 为真：编号 i
 * - 变量 x_i 为假：编号 i + n
 * - 总节点数：2 * n
 * ================================================
 */
struct TwoSAT {
    int n;
    TarjanSCC g;

    TwoSAT(int n) : n(n), g(2 * n) {}

    // 添加边：u -> v
    void add_edge(int u, int v) {
        g.add_edge(u, v);
    }

    // 添加约束：(x_a == val_a) OR (x_b == val_b)
    // 等价于：(x_a != val_a) -> (x_b == val_b) 且 (x_b != val_b) -> (x_a == val_a)
    void add_clause(int a, bool val_a, int b, bool val_b) {
        // 节点编号：如果 val 为 true 则为 a，否则为 a + n
        int node_a = val_a ? a : a + n;
        int node_b = val_b ? b : b + n;
        
        // 取反节点：如果 val 为 true 则取反为 a + n，否则为 a
        int neg_node_a = val_a ? a + n : a;
        int neg_node_b = val_b ? b + n : b;

        // 添加蕴含边
        add_edge(neg_node_a, node_b);
        add_edge(neg_node_b, node_a);
    }

    // 求解并返回解（若存在）
    // 返回值：空 vector 表示无解，否则返回长度为 n 的 0/1 数组
    vector<int> solve() {
        g.run();
        
        // 检查是否存在矛盾：x_i 和 x_i+n 在同一个 SCC 中
        for (int i = 0; i < n; i++) {
            if (g.comp[i] == g.comp[i + n]) {
                return {}; // 无解
            }
        }

        // 构造解：选择 SCC 编号较小的状态（逆拓扑序）
        vector<int> res(n);
        for (int i = 0; i < n; i++) {
            // 如果 i (真) 的 SCC 编号 < i+n (假) 的 SCC 编号，则 x_i = 1
            // 否则 x_i = 0
            res[i] = (g.comp[i] < g.comp[i + n]) ? 1 : 0;
        }
        return res;
    }
};

// ================== 主函数（以洛谷 P4782 为例） ==================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    TwoSAT solver(n);

    for (int i = 0; i < m; i++) {
        int a, va, b, vb;
        cin >> a >> va >> b >> vb;
        // 题目输入是 1-indexed，转换为 0-indexed
        a--; b--;
        
        // 添加约束：(x_a == va) OR (x_b == vb)
        solver.add_clause(a, va, b, vb);
    }

    vector<int> res = solver.solve();

    if (res.empty()) {
        cout << "IMPOSSIBLE\n";
    } else {
        cout << "POSSIBLE\n";
        for (int i = 0; i < n; i++) {
            cout << res[i] << " ";
        }
        cout << "\n";
    }

    return 0;
}

/*
 * ============================ 强连通分量（SCC）============================
 * 【问题描述】
 * 给定有向图 G=(V,E)，|V|=n，|E|=m。
 *   强连通：顶点 u,v 互相可达（u→v 与 v→u 的路径都存在）。
 *   强连通分量（SCC）：满足「内部任意两点互相可达」的极大子图，
 *                      再加任何一个顶点都会破坏强连通性。
 * 【任务】把 V 划分成若干 SCC，输出每个 SCC 的成员（通常还需按拓扑序输出）。
 *
 * 【关键性质】
 *   1. 每个顶点恰好属于一个 SCC，SCC 构成 V 的一个划分。
 *   2. 缩点（每个 SCC 缩成一个超级点）后得到的图一定是 DAG。
 *   3. 图中每个环都完整落在某个 SCC 内部。
 *
 * 【算法对比】
 *   Tarjan   : 一次 DFS，dfn(时间戳) + low(追溯值) 识别 SCC 根，O(n+m)
 *   Kosaraju : 正反图各 DFS 一次，利用逆后序，O(n+m)
 *   Gabow    : 双栈优化 Tarjan 的标记过程，O(n+m)
 *
 * 【本实现要点】
 *   - dfn[u] == low[u] 时，u 是某个 SCC 的根，开始弹栈。
 *   - 回边/交叉边更新时用 dfn[v] 而非 low[v]（v 已在栈中，说明构成环）。
 *   - ★ SCC 编号 scc_cnt 递增的顺序就是「逆拓扑序」：
 *     先弹出的 SCC 在缩点 DAG 中位于下游，后弹出的位于上游。
 *     因此对边 (a,b) 且 comp[a]!=comp[b]，必有 comp[a] > comp[b]。
 *     倒序输出 comp 编号即得拓扑序。
 * ========================================================================
 */
 
 /*
 * ============================== 2-SAT 问题 ==============================
 * 【问题描述】
 * 给定 n 个布尔变量 x_1..x_n 和 m 个约束，每个约束是「两个文字的或」：
 *     (x_a ∨ ¬x_b),  (¬x_i ∨ ¬x_j),  (x_p ∨ x_q) ...
 * 文字 = 变量本身或其否定。整个公式是合取范式（CNF）：
 *     F = C_1 ∧ C_2 ∧ ... ∧ C_m
 * 目标：给每个变量赋 true/false 使所有子句同时为真；无解则判定无解。
 *
 * 【与一般 SAT 的关系】
 *   - 一般 k-SAT（k≥3）是 NP 完全的。
 *   - 2-SAT 可在 O(n+m) 线性时间内精确求解。
 *   - 优化版 MAX 2-SAT（最大化满足子句数）仍是 NP 完全的。
 *
 * 【建模：蕴含图】
 *   核心技巧：(a ∨ b) ⟺ (¬a → b) ∧ (¬b → a)
 *   语义：若 a 为假则 b 必为真；若 b 为假则 a 必为真。
 *   节点：变量 x_i 拆成两个节点
 *         i     代表 x_i = true
 *         i + n 代表 x_i = false
 *   共 2n 个节点。每个子句产生 2 条有向边。
 *
 *   ┌──────────────┬────────────────────────────┐
 *   │ 原子句        │ 建边                        │
 *   ├──────────────┼────────────────────────────┤
 *   │ a ∨ b        │ ¬a → b ,  ¬b → a           │
 *   │ ¬a ∨ b       │ a → b  ,  ¬b → ¬a          │
 *   │ a ∨ ¬b       │ ¬a → ¬b,  b  → a           │
 *   │ ¬a ∨ ¬b      │ a → ¬b ,  b  → ¬a          │
 *   └──────────────┴────────────────────────────┘
 *   图天然对称：存在边 u→v ⟺ 存在边 ¬v→¬u。
 *
 * 【判定与构造】
 *   无解 ⟺ 存在某个 i 使 comp[i] == comp[i+n]（x_i 与 ¬x_i 同 SCC）。
 *     原因：同一 SCC 内文字互相可达，蕴含传递要求它们同真同假，
 *           于是 x_i ≡ ¬x_i，矛盾。
 *   构造：按 SCC 的逆拓扑序贪心。因 Tarjan 编号本身是逆拓扑序，
 *         取 comp 编号更小者（拓扑序更靠后）为真：
 *         x_i = (comp[i] < comp[i+n]) ? true : false
 *
 * 【复杂度】建图 O(m) + Tarjan O(n+m) = O(n+m)
 *
 * 【常见建模套路速查】
 *   1. 「A 和 B 至少选一个」      → add_clause(A, true,  B, true)
 *   2. 「A 和 B 不能同时选」      → add_clause(A, false, B, false)   ★易错
 *   3. 「若选 A 则必须选 B」      → add_clause(A, false, B, true)
 *   4. 「A 和 B 必须同选或同不选」→ 两个子句：(¬A∨B) 与 (A∨¬B)
 *   5. 「恰好选一个」（互斥且必选）→ (A∨B) 加 (¬A∨¬B)
 *   6. 「强制 A 为真」            → add_clause(A, true, A, true)
 * ========================================================================
 */
 
 /*
 * ============================================================================================
 * File Description: C++ Template for Competitive Programming (CPC)
 * Template Source: https://github.com/moi9012/CPC-Template
 * Compiler Version: C++17 / C++20
 * Author Notes: Includes common headers, macros, fast I/O, and frequently used data structures.
 *               Suitable for platforms like Codeforces and CodeChef.
 * ============================================================================================
 */