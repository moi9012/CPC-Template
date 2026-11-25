struct Trie {
    vector<array<int, 26>> nxt;
    vector<int> cnt;
    int tot;
    Trie() {
        nxt.push_back(array<int, 26>());
        nxt[0].fill(0);
        cnt.push_back(0);
        tot = 0;
    }
    void insert(const string &s) {
        int p = 0;
        for (char c : s) {
            int i = c - 'a';
            if (!nxt[p][i]) {
                nxt.push_back(array<int, 26>());
                nxt.back().fill(0);
                cnt.push_back(0);
                nxt[p][i] = ++tot;
            }
            p = nxt[p][i];
        }
        cnt[p]++;
    }
    int query(const string &s) {
        int p = 0;
        for (char c : s) {
            int i = c - 'a';
            if (!nxt[p][i]) return 0;
            p = nxt[p][i];
        }
        return cnt[p];
    }
};