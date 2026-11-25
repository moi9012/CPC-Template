struct Block {
    vector<ll> a, add, sum;
    int block_size;
    Block(vector<ll> &arr) : a(arr) {
        int n = a.size();
        block_size = max(1, (int)sqrt(n));
        add.assign(n, 0);
        sum.assign(n, 0);
        for (int i = 0; i < n; ++i) {
            sum[i / block_size] += a[i];
        }
    }
    void update(int l, int r, ll c) {
        int bl = l / block_size, br = r / block_size;
        if (bl == br) {
            for (int i = l; i <= r; ++i) a[i] += c;
            sum[bl] += c * (r - l + 1);
        } else {
            for (int i = l; i < (bl + 1) * block_size; ++i) a[i] += c;
            sum[bl] += c * ((bl + 1) * block_size - l);
            for (int i = br * block_size; i <= r; ++i) a[i] += c;
            sum[br] += c * (r - br * block_size + 1);
            for (int i = bl + 1; i < br; ++i) {
                add[i] += c;
                sum[i] += c * block_size;
            }
        }
    }
    ll query(int l, int r) {
        int bl = l / block_size, br = r / block_size;
        ll res = 0;
        if (bl == br) {
            for (int i = l; i <= r; ++i) res += a[i] + add[bl];
        } else {
            for (int i = l; i < (bl + 1) * block_size; ++i) res += a[i] + add[bl];
            for (int i = br * block_size; i <= r; ++i) res += a[i] + add[br];
            for (int i = bl + 1; i < br; ++i) res += sum[i];
        }
        return res;
    }
};