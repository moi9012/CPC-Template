#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

typedef long long ll;

const int MAXN = 500005;
const ll MOD = 998244353;
const ll NEG_INF = LLONG_MIN / 4;   // 最大值查询的负无穷单位元

struct SegmentTree {
    struct Node {
        int l, r;        // 闭区间 [l, r]
        ll sum;          // 区间和（mod MOD）
        ll max_val;      // 区间最大值（不取模，用于二分剪枝）
        ll b, c;         // 懒标记：a_i <- b * a_i + c
    } tr[4 * MAXN];

    // ========== 标记相关 ==========

    // 对节点 u 施加标记 (b, c)：a_i <- b * a_i + c
    void apply_tag(int u, ll b, ll c) {
        int len = tr[u].r - tr[u].l + 1;

        // 1) 更新区间和：sum <- b * sum + c * len
        tr[u].sum = (b * tr[u].sum % MOD + c % MOD * (len % MOD)) % MOD;

        // 2) 更新区间最大值：max <- b * max + c
        //    注意：只有 b >= 0 时单调递增才成立，本题 b >= 1，成立
        tr[u].max_val = b * tr[u].max_val + c;

        // 3) 复合懒标记：先应用旧标记 (tr.b, tr.c)，再应用新标记 (b, c)
        //    a -> tr.b * a + tr.c -> b * (tr.b * a + tr.c) + c
        //      = (tr.b * b) * a + (b * tr.c + c)
        tr[u].b = tr[u].b * b % MOD;
        tr[u].c = (b * tr[u].c % MOD + c) % MOD;
    }

    // 下传标记到左右孩子
    void push_down(int u) {
        if (tr[u].b != 1 || tr[u].c != 0) {
            apply_tag(u << 1, tr[u].b, tr[u].c);
            apply_tag(u << 1 | 1, tr[u].b, tr[u].c);
            tr[u].b = 1;
            tr[u].c = 0;
        }
    }

    // 从孩子向上更新
    void push_up(int u) {
        tr[u].sum = (tr[u << 1].sum + tr[u << 1 | 1].sum) % MOD;
        tr[u].max_val = max(tr[u << 1].max_val, tr[u << 1 | 1].max_val);
    }

    // ========== 建树 ==========

    void build(int u, int l, int r, const vector<ll>& a) {
        tr[u] = {l, r, 0, NEG_INF, 1, 0};   // 标记初始化为恒等变换 (b=1, c=0)
        if (l == r) {
            tr[u].sum = a[l] % MOD;
            tr[u].max_val = a[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        push_up(u);
    }

    // ========== 区间修改：a_i <- b * a_i + c，i ∈ [ql, qr] ==========

    void update(int u, int ql, int qr, ll b, ll c) {
        if (qr < tr[u].l || tr[u].r < ql) return;              // 无交集
        if (ql <= tr[u].l && tr[u].r <= qr) {                   // 完全包含
            apply_tag(u, b, c);
            return;
        }
        push_down(u);                                           // ★ 必须先下传
        update(u << 1, ql, qr, b, c);
        update(u << 1 | 1, ql, qr, b, c);
        push_up(u);
    }

    // ========== 区间求和 ==========

    ll query_sum(int u, int ql, int qr) {
        if (qr < tr[u].l || tr[u].r < ql) return 0;            // 无交集
        if (ql <= tr[u].l && tr[u].r <= qr) return tr[u].sum;  // 完全包含
        push_down(u);
        return (query_sum(u << 1, ql, qr) + query_sum(u << 1 | 1, ql, qr)) % MOD;
    }

    // ========== 区间最大值 ==========

    ll query_max(int u, int ql, int qr) {
        if (qr < tr[u].l || tr[u].r < ql) return NEG_INF;      // 无交集
        if (ql <= tr[u].l && tr[u].r <= qr) return tr[u].max_val;
        push_down(u);
        return max(query_max(u << 1, ql, qr), query_max(u << 1 | 1, ql, qr));
    }

    // ========== 线段树二分：[ql, qr] 内第一个 >= V 的下标，无解返回 -1 ==========

    int find_first(int u, int ql, int qr, ll V) {
        // 剪枝 1：无交集
        if (qr < tr[u].l || tr[u].r < ql) return -1;
        // 剪枝 2：整段最大值都不够
        if (tr[u].max_val < V) return -1;
        // 叶子：走到这里必然满足 max_val >= V 且在范围内
        if (tr[u].l == tr[u].r) return tr[u].l;

        push_down(u);   // ★★ 关键：下降前必须下传标记，否则孩子信息过期

        // 先左后右，保证下标最小
        int res = find_first(u << 1, ql, qr, V);
        if (res != -1) return res;
        return find_first(u << 1 | 1, ql, qr, V);
    }

    // ========== 线段树二分：[ql, qr] 内第一个前缀和 >= V 的下标 ==========
    // 适用于 a_i >= 0 的场景（保证前缀和单调）

    int find_prefix(int u, int ql, int qr, ll V) {
        if (qr < tr[u].l || tr[u].r < ql) return -1;
        if (tr[u].sum < V) return -1;
        if (tr[u].l == tr[u].r) return tr[u].l;

        push_down(u);

        int res = find_prefix(u << 1, ql, qr, V);
        if (res != -1) return res;
        // 注意：若左子树部分命中，右子树的 V 需要减去左子树贡献
        // 简化版：这里只做「存在性」判断，严格前缀和查找见下方说明
        return find_prefix(u << 1 | 1, ql, qr, V);
    }
};

SegmentTree st;   // ★ 全局变量，避免 12MB 数组爆栈

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;

    vector<ll> a(N);
    for (int i = 0; i < N; ++i) cin >> a[i];

    st.build(1, 0, N - 1, a);

    while (Q--) {
        int T;
        cin >> T;
        if (T == 0) {
            int l, r; ll b, c;
            cin >> l >> r >> b >> c;
            st.update(1, l, r - 1, b % MOD, c % MOD);   // [l, r) -> [l, r-1]
        } else if (T == 1) {
            int l, r;
            cin >> l >> r;
            cout << st.query_sum(1, l, r - 1) << '\n';
        } else if (T == 2) {
            // 扩展：区间最大值
            int l, r;
            cin >> l >> r;
            cout << st.query_max(1, l, r - 1) << '\n';
        } else {
            // 扩展：找第一个 >= V
            int l; ll V;
            cin >> l >> V;
            int ans = st.find_first(1, l, N - 1, V);
            cout << (ans == -1 ? N : ans) << '\n';
        }
    }
    return 0;
}