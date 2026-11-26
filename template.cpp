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

using namespace std;

using ll = long long;
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

void solve() {
    // Write the logic for a single test case here
    int n;
    if (!(cin >> n)) return;
    
    // Example logic
    cout << n * 2 << endl;
}

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

/*
 * ============================================================================================
 * File Description: C++ Template for Competitive Programming (CPC)
 * Template Source: https://github.com/moi9012/CPC-Template
 * Compiler Version: C++17 / C++20
 * Author Notes: Includes common headers, macros, fast I/O, and frequently used data structures.
 *               Suitable for platforms like Codeforces and CodeChef.
 * ============================================================================================
 */