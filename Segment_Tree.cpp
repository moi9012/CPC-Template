struct SegmentTree {
    struct Node {
        int l, r;
        ll sum, add;
    } tr[400005];

    void push_up(int u) {
        tr[u].sum = tr[u << 1].sum + tr[u << 1 | 1].sum;
    }
    void push_down(int u) {
        if (tr[u].add) {
            auto &L = tr[u << 1], &R = tr[u << 1 | 1];
            L.sum += tr[u].add * (L.r - L.l + 1);
            L.add += tr[u].add;
            R.sum += tr[u].add * (R.r - R.l + 1);
            R.add += tr[u].add;
            tr[u].add = 0;
        }
    }
    void build(int u, int l, int r, vector<ll> &a) {
        tr[u] = {l, r, a[l], 0};
        if (l == r) return;
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        push_up(u);
    }
    void update(int u, int l, int r, ll c) {
        if (tr[u].l >= l && tr[u].r <= r) {
            tr[u].sum += c * (tr[u].r - tr[u].l + 1);
            tr[u].add += c;
            return;
        }
        push_down(u);
        int mid = (tr[u].l + tr[u].r) >> 1;
        if (l <= mid) update(u << 1, l, r, c);
        if (r > mid) update(u << 1 | 1, l, r, c);
        push_up(u);
    }
    ll query(int u, int l, int r) {
        if (tr[u].l >= l && tr[u].r <= r) return tr[u].sum;
        push_down(u);
        int mid = (tr[u].l + tr[u].r) >> 1;
        ll res = 0;
        if (l <= mid) res += query(u << 1, l, r);
        if (r > mid) res += query(u << 1 | 1, l, r);
        return res;
    }
};