// 示例：统计 [1, N] 中不包含 '4' 的数字个数
// 需根据具体题目修改 check 逻辑和状态
ll memo[20][2]; // [位置][是否受限]
string s;

ll dfs(int pos, bool limit) {
    if (pos == s.size()) return 1;
    if (!limit && memo[pos][1] != -1) return memo[pos][1];
    
    int up = limit ? s[pos] - '0' : 9;
    ll res = 0;
    for (int i = 0; i <= up; ++i) {
        if (i == 4) continue; // 题目条件：不含4
        res += dfs(pos + 1, limit && (i == up));
    }
    
    if (!limit) memo[pos][1] = res;
    return res;
}

ll solve(ll N) {
    s = to_string(N);
    memset(memo, -1, sizeof(memo));
    return dfs(0, true);
}