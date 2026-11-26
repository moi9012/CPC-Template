#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <cstring>
using namespace std;

/*
 * ================== 1. KMP 算法 ==================
 * 【核心思想】利用模式串自身的对称性（前缀函数），在匹配失败时避免主串指针回溯。
 * 【时间复杂度】O(n + m)
 */
struct KMP {
    vector<int> nxt; // nxt[i]: 模式串前 i 个字符组成的子串的最长相等前后缀长度
    
    KMP(const string &p) {
        int m = p.size();
        nxt.assign(m + 1, 0);
        // 构建 nxt 数组
        for (int i = 1, j = 0; i < m; ++i) {
            while (j > 0 && p[i] != p[j]) j = nxt[j];
            if (p[i] == p[j]) ++j;
            nxt[i + 1] = j;
        }
    }
    
    // 返回模式串在文本串 s 中第一次出现的下标，未找到返回 -1
    int search(const string &s, const string &p) {
        int n = s.size(), m = p.size();
        for (int i = 0, j = 0; i < n; ++i) {
            while (j > 0 && s[i] != p[j]) j = nxt[j];
            if (s[i] == p[j]) ++j;
            if (j == m) return i - m + 1; // 匹配成功
        }
        return -1;
    }
};

/*
 * ================== 2. Manacher 算法 ==================
 * 【核心思想】通过插入分隔符将奇偶回文统一为奇数回文，利用回文对称性避免重复暴力匹配。
 * 【时间复杂度】O(n)
 */
struct Manacher {
    string t;
    vector<int> p; // p[i]: 以 t[i] 为中心的最长回文半径（包含中心）
    
    Manacher(const string &s) {
        // 预处理：插入 '#'，首尾加特殊字符防越界
        t = "^#";
        for (char c : s) { t += c; t += '#'; }
        t += "$";
        
        int n = t.size();
        p.assign(n, 0);
        int mid = 0, right = 0; // right 为当前最右回文边界，mid 为其中心
        
        for (int i = 1; i < n - 1; ++i) {
            // 利用对称性初始化 p[i]
            if (i < right) p[i] = min(right - i, p[2 * mid - i]);
            else p[i] = 1;
            
            // 暴力扩展
            while (t[i + p[i]] == t[i - p[i]]) ++p[i];
            
            // 更新最右边界
            if (i + p[i] > right) {
                right = i + p[i];
                mid = i;
            }
        }
    }
    
    // 返回原串的最长回文子串长度
    int longestPalindrome() {
        int max_len = 0;
        for (int len : p) max_len = max(max_len, len - 1);
        return max_len;
    }
};

/*
 * ================== 3. AC 自动机 ==================
 * 【核心思想】Trie 树 + KMP 的 fail 指针，实现多模式串的同时匹配。
 * 【时间复杂度】建树 O(总长度 * Σ)，匹配 O(文本串长度)
 */
struct ACAutomaton {
    static const int ALPHA = 26;
    vector<vector<int>> trie;
    vector<int> fail, cnt; // cnt[u]: 以节点 u 为终点的模式串数量
    
    ACAutomaton() {
        trie.push_back(vector<int>(ALPHA, 0));
        fail.push_back(0);
        cnt.push_back(0);
    }
    
    // 插入模式串
    void insert(const string &s) {
        int u = 0;
        for (char c : s) {
            int v = c - 'a';
            if (!trie[u][v]) {
                trie[u][v] = trie.size();
                trie.push_back(vector<int>(ALPHA, 0));
                fail.push_back(0);
                cnt.push_back(0);
            }
            u = trie[u][v];
        }
        cnt[u]++;
    }
    
    // 构建 fail 指针（BFS）
    void build() {
        queue<int> q;
        for (int i = 0; i < ALPHA; ++i) {
            if (trie[0][i]) q.push(trie[0][i]);
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int i = 0; i < ALPHA; ++i) {
                int v = trie[u][i];
                if (v) {
                    // 核心：fail[v] 指向 fail[u] 的对应字符转移节点
                    fail[v] = trie[fail[u]][i];
                    // 路径压缩优化：将当前节点的计数累加到 fail 节点，方便后续统计
                    cnt[fail[v]] += cnt[v]; 
                    q.push(v);
                } else {
                    // 优化：若无转移，直接指向 fail 节点的转移（Trie 图化）
                    trie[u][i] = trie[fail[u]][i];
                }
            }
        }
    }
    
    // 查询文本串中所有模式串出现的总次数
    long long query(const string &s) {
        long long res = 0;
        int u = 0;
        for (char c : s) {
            u = trie[u][c - 'a'];
            res += cnt[u]; // 累加当前节点及 fail 链上所有模式串的计数
        }
        return res;
    }
};

/*
 * ================== 4. 后缀数组 (SA) ==================
 * 【核心思想】倍增 + 基数排序，对所有后缀进行字典序排序，并求相邻后缀的最长公共前缀 (LCP)。
 * 【时间复杂度】O(n log n)
 */
struct SuffixArray {
    string s;
    vector<int> sa, rk, lcp;
    int n;
    
    SuffixArray(const string &_s) : s(_s), n(_s.size()) {
        sa.resize(n); rk.resize(n); lcp.resize(n);
        build();
    }
    
    void build() {
        vector<int> tmp(n), cnt(max(256, n), 0);
        // 初始排序（按单字符）
        for (int i = 0; i < n; ++i) rk[i] = s[i], cnt[rk[i]]++;
        for (int i = 1; i < cnt.size(); ++i) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; --i) sa[--cnt[rk[i]]] = i;
        
        // 倍增
        for (int k = 1; k < n; k <<= 1) {
            int p = 0;
            // 第二关键字排序（长度不足 k 的排在前面）
            for (int i = n - k; i < n; ++i) tmp[p++] = i;
            for (int i = 0; i < n; ++i) if (sa[i] >= k) tmp[p++] = sa[i] - k;
            
            // 第一关键字基数排序
            fill(cnt.begin(), cnt.end(), 0);
            for (int i = 0; i < n; ++i) cnt[rk[i]]++;
            for (int i = 1; i < cnt.size(); ++i) cnt[i] += cnt[i - 1];
            for (int i = n - 1; i >= 0; --i) sa[--cnt[rk[tmp[i]]]] = tmp[i];
            
            // 重新计算 rk
            tmp[sa[0]] = 0;
            for (int i = 1; i < n; ++i) {
                int a = sa[i - 1], b = sa[i];
                // 比较 (rk[a], rk[a+k]) 与 (rk[b], rk[b+k])
                bool same = (rk[a] == rk[b]) && 
                            ((a + k < n ? rk[a + k] : -1) == (b + k < n ? rk[b + k] : -1));
                tmp[b] = same ? tmp[a] : tmp[a] + 1;
            }
            rk = tmp;
            if (rk[sa[n - 1]] == n - 1) break; // 所有排名互不相同，提前结束
        }
        
        // 求 LCP 数组 (Kasai 算法)
        int h = 0;
        for (int i = 0; i < n; ++i) {
            if (rk[i] > 0) {
                int j = sa[rk[i] - 1];
                while (i + h < n && j + h < n && s[i + h] == s[j + h]) ++h;
                lcp[rk[i]] = h;
                if (h) --h;
            } else h = 0;
        }
    }
};

// ================== 主函数测试 ==================
int main() {
    // 1. KMP 测试
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABC";
    KMP kmp(pattern);
    cout << "KMP Find at: " << kmp.search(text, pattern) << endl; // 应为 10
    
    // 2. Manacher 测试
    string pal = "babad";
    Manacher man(pal);
    cout << "Manacher Max Palindrome: " << man.longestPalindrome() << endl; // 应为 3 ("bab" 或 "aba")
    
    // 3. AC 自动机测试
    ACAutomaton ac;
    ac.insert("he");
    ac.insert("she");
    ac.insert("his");
    ac.insert("hers");
    ac.build();
    cout << "AC Automaton Total Matches: " << ac.query("ushers") << endl; // 应为 3 (she, he, hers)
    
    // 4. 后缀数组测试
    string sa_str = "banana";
    SuffixArray sa(sa_str);
    cout << "SA of 'banana': ";
    for (int i : sa.sa) cout << i << " "; // 应为 5 3 1 0 4 2
    cout << endl;
    
    return 0;
}

/*
 * ============================================================================================
 * File Description: C++ Template for Competitive Programming (CPC)
 * Template Source: https://github.com/moi9012/CPC-Template
 * Compiler Version: C++17 / C++20
 * Author Notes: Includes common headers, macros, fast I/O, and frequently used data structures.
 *               Suitable for platforms like Codeforces and CodeChef.
 * ============================================================================================
 */