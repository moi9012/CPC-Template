vector<int> primes;
vector<bool> is_comp;

void get_primes(int n) {
    is_comp.assign(n + 1, false);
    for (int i = 2; i <= n; ++i) {
        if (!is_comp[i]) primes.push_back(i);
        for (int p : primes) {
            if ((ll)i * p > n) break;
            is_comp[i * p] = true;
            if (i % p == 0) break; // 保证每个数只被最小质因子筛掉
        }
    }
}