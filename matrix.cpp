#include <vector>
#include <iostream>
using namespace std;

typedef long long ll;

struct Matrix {
    int n, m;
    vector<vector<ll>> mat;
    Matrix(int _n, int _m) : n(_n), m(_m) {
        mat.assign(n, vector<ll>(m, 0));
    }
};

/*
 * ================== 矩阵乘法 ==================
 * 【核心思想】
 * C[i][j] = sum(A[i][k] * B[k][j])
 * 
 * 【循环顺序优化】
 * 采用 i-k-j 而非 i-j-k 的循环顺序。
 * 在 i-k-j 顺序下，内层循环遍历 B[k][j] 和 C[i][j] 时，
 * 内存访问是连续的（行优先存储），能极大提高 CPU Cache 命中率。
 * 同时，if (A.mat[i][k]) 的剪枝在矩阵稀疏时能大幅减少计算量。
 * 
 * 【时间复杂度】
 * O(n^3)
 * ==============================================
 */
Matrix mul(const Matrix &A, const Matrix &B, ll p) {
    Matrix C(A.n, B.m);
    for (int i = 0; i < A.n; ++i)
        for (int k = 0; k < A.m; ++k)
            if (A.mat[i][k]) // 剪枝：若 A[i][k] 为 0 则无需计算
                for (int j = 0; j < B.m; ++j)
                    C.mat[i][j] = (C.mat[i][j] + A.mat[i][k] * B.mat[k][j]) % p;
    return C;
}

/*
 * ================== 矩阵快速幂 ==================
 * 【核心思想】
 * 利用快速幂的思想计算 A^n。
 * 将指数 n 进行二进制拆分，例如 A^13 = A^(8+4+1) = A^8 * A^4 * A^1。
 * 通过不断平方底数（A -> A^2 -> A^4 -> A^8...），并在二进制位为 1 时累乘到结果中。
 * 
 * 【时间复杂度】
 * O(k^3 log n)，其中 k 为矩阵阶数，n 为幂次。
 * 
 * 【常见应用】
 * 1. 斐波那契数列：构造 [[1,1],[1,0]]，求 F(n) 在 O(log n) 内完成。
 * 2. 线性递推数列：如 f(n) = a*f(n-1) + b*f(n-2) + c，通过构造矩阵求解。
 * 3. 图上路径计数：邻接矩阵的 k 次幂表示两点间长度为 k 的路径数。
 * 4. 状态转移优化：将 O(n) 的状态转移方程优化为 O(log n)。
 * ================================================
 */
Matrix mat_pow(Matrix A, ll n, ll p) {
    // 初始化结果为单位矩阵（对角线为 1）
    Matrix res(A.n, A.n);
    for (int i = 0; i < A.n; ++i) res.mat[i][i] = 1;
    
    // 快速幂主体
    while (n > 0) {
        if (n & 1) res = mul(res, A, p); // 当前二进制位为 1，累乘
        A = mul(A, A, p);                // 底数平方
        n >>= 1;                         // 指数右移
    }
    return res;
}