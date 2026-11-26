#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

/*
 * ================== 字典树 (Trie) ==================
 * 【核心思想】利用字符串的公共前缀来节约存储空间，将字符串查询的时间复杂度降为 O(L)。
 * 【时间复杂度】插入/查询 O(L)，L 为字符串长度。
 */
struct Trie {
    // 节点结构：children 指向子节点，is_end 标记是否为完整单词的结尾，prefix_cnt 记录经过该节点的前缀数量
    struct Node {
        vector<int> children;
        bool is_end;
        int prefix_cnt;
        Node(int alpha_size = 26) : children(alpha_size, -1), is_end(false), prefix_cnt(0) {}
    };
    
    vector<Node> nodes;
    int alpha_size;
    
    Trie(int alpha_size = 26) : alpha_size(alpha_size) {
        nodes.push_back(Node(alpha_size)); // 初始化根节点
    }
    
    // 1. 插入字符串
    void insert(const string &s) {
        int cur = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (nodes[cur].children[idx] == -1) {
                nodes.push_back(Node(alpha_size));
                nodes[cur].children[idx] = nodes.size() - 1;
            }
            cur = nodes[cur].children[idx];
            nodes[cur].prefix_cnt++; // 更新前缀计数
        }
        nodes[cur].is_end = true;
    }
    
    // 2. 查找字符串是否完整存在
    bool search(const string &s) {
        int cur = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (nodes[cur].children[idx] == -1) return false;
            cur = nodes[cur].children[idx];
        }
        return nodes[cur].is_end;
    }
    
    // 3. 查找是否存在以 s 为前缀的字符串
    bool startsWith(const string &s) {
        int cur = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (nodes[cur].children[idx] == -1) return false;
            cur = nodes[cur].children[idx];
        }
        return true;
    }
    
    // 4. 统计以 prefix 为前缀的字符串数量
    int countPrefix(const string &prefix) {
        int cur = 0;
        for (char c : prefix) {
            int idx = c - 'a';
            if (nodes[cur].children[idx] == -1) return 0;
            cur = nodes[cur].children[idx];
        }
        return nodes[cur].prefix_cnt;
    }
};

/*
 * ================== 01-Trie (异或最值扩展) ==================
 * 【核心思想】将整数按二进制位从高位到低位插入 Trie，贪心选择相反位以获得最大异或值。
 * 【应用场景】求数组中两个数的最大异或值、区间最大异或和等。
 */
struct Trie01 {
    struct Node {
        int children[2];
        Node() { children[0] = children[1] = -1; }
    };
    vector<Node> nodes;
    
    Trie01() { nodes.push_back(Node()); }
    
    // 插入数字（默认 31 位，足够覆盖 int 范围）
    void insert(int num, int bits = 31) {
        int cur = 0;
        for (int i = bits - 1; i >= 0; --i) {
            int bit = (num >> i) & 1;
            if (nodes[cur].children[bit] == -1) {
                nodes.push_back(Node());
                nodes[cur].children[bit] = nodes.size() - 1;
            }
            cur = nodes[cur].children[bit];
        }
    }
    
    // 查询与 num 异或能得到的最大值
    int getMaxXor(int num, int bits = 31) {
        int cur = 0, res = 0;
        for (int i = bits - 1; i >= 0; --i) {
            int bit = (num >> i) & 1;
            // 贪心：尽量走与当前位相反的路径
            int target = 1 - bit; 
            if (nodes[cur].children[target] != -1) {
                res |= (1 << i); // 该位能取到 1
                cur = nodes[cur].children[target];
            } else {
                cur = nodes[cur].children[bit];
            }
        }
        return res;
    }
};

// ================== 主函数测试 ==================
int main() {
    // 1. 基础 Trie 测试
    Trie trie;
    trie.insert("apple");
    trie.insert("app");
    trie.insert("banana");
    
    cout << "Search 'app': " << (trie.search("app") ? "true" : "false") << endl; // true
    cout << "Search 'appl': " << (trie.search("appl") ? "true" : "false") << endl; // false
    cout << "Starts with 'ban': " << (trie.startsWith("ban") ? "true" : "false") << endl; // true
    cout << "Count prefix 'ap': " << trie.countPrefix("ap") << endl; // 2 ("apple", "app")
    
    // 2. 01-Trie 测试：求数组中两数的最大异或值
    vector<int> nums = {3, 10, 5, 25, 2, 8};
    Trie01 t01;
    for (int num : nums) t01.insert(num);
    
    int max_xor = 0;
    for (int num : nums) {
        max_xor = max(max_xor, t01.getMaxXor(num));
    }
    cout << "Max XOR of two numbers: " << max_xor << endl; // 应为 28 (5 ^ 25)
    
    return 0;
}