struct Matrix {
    int n, m;
    vector<vector<ll>> mat;
    Matrix(int _n, int _m) : n(_n), m(_m) {
        mat.assign(n, vector<ll>(m, 0));
    }
};

Matrix mul(const Matrix &A, const Matrix &B, ll p) {
    Matrix C(A.n, B.m);
    for (int i = 0; i < A.n; ++i)
        for (int k = 0; k < A.m; ++k)
            if (A.mat[i][k])
                for (int j = 0; j < B.m; ++j)
                    C.mat[i][j] = (C.mat[i][j] + A.mat[i][k] * B.mat[k][j]) % p;
    return C;
}