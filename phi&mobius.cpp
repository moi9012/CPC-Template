// 线性筛同时求欧拉函数 phi 和莫比乌斯函数 mu
vector<int> phi, mu;
void get_num_theory(int n) {
    phi.resize(n + 1);
    mu.resize(n + 1);
    is_comp.assign(n + 1, false);
    phi[1] = 1;
    mu[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            primes.push_back(i);
            phi[i] = i - 1;
            mu[i] = -1;
        }
        for (int p : primes) {
            if ((ll)i * p > n) break;
            is_comp[i * p] = true;
            if (i % p == 0) {
                phi[i * p] = phi[i] * p;
                mu[i * p] = 0; // 含有平方因子
                break;
            } else {
                phi[i * p] = phi[i] * (p - 1);
                mu[i * p] = -mu[i];
            }
        }
    }
}