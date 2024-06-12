#include <cassert>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

typedef std::vector<std::pair<int, int> > S2d;
typedef std::vector<std::vector<float> > polynomial_2d;
typedef std::vector<std::vector<std::complex<float> > > complex_polynomial_2d;

const float PI = acos(-1.0f);

// 快速傅里叶变换
void fft1d(std::vector<std::complex<float> > &a, bool invert) {
    int n = a.size();
    if (n == 1)
        return;

    std::vector<std::complex<float> > a0(n / 2), a1(n / 2);
    for (int i = 0; 2 * i < n; i++) {
        a0[i] = a[i * 2];
        a1[i] = a[i * 2 + 1];
    }
    fft1d(a0, invert);
    fft1d(a1, invert);

    float ang = 2 * PI / n * (invert ? -1 : 1);
    std::complex<float> w(1), wn(cos(ang), sin(ang));
    for (int i = 0; 2 * i < n; i++) {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];
        if (invert) {
            a[i] /= 2;
            a[i + n / 2] /= 2;
        }
        w *= wn;
    }
}

// 二维FFT
void fft2d(complex_polynomial_2d &a, bool invert) {
    int n = a.size();
    int m = a[0].size();

    // 对每一行进行FFT
    for (int i = 0; i < n; i++) {
        fft1d(a[i], invert);
    }

    // 对每一列进行FFT
    for (int j = 0; j < m; j++) {
        std::vector<std::complex<float> > column(n);
        for (int i = 0; i < n; i++) {
            column[i] = a[i][j];
        }
        fft1d(column, invert);
        for (int i = 0; i < n; i++) {
            a[i][j] = column[i];
        }
    }
}

// 将S2d类型转换为多项式系数矩阵
polynomial_2d convert_to_polynomial_2d(const S2d &s, int &n, int &m) {
    // 确定矩阵大小
    int max_i = 0, max_j = 0;
    for (const auto &p: s) {
        if (p.first > max_i)
            max_i = p.first;
        if (p.second > max_j)
            max_j = p.second;
    }

    // 确保大小为2的幂次
    n = 1;
    while (n <= max_i)
        n <<= 1;
    m = 1;
    while (m <= max_j)
        m <<= 1;

    // 初始化多项式系数矩阵
    polynomial_2d poly(n, std::vector<float>(m, 0));
    for (const auto &p: s) {
        poly[p.first][p.second] = 1;
    }

    return poly;
}

// 多项式卷积
polynomial_2d polynomial_convolution_2d(const polynomial_2d &a,
                                        const polynomial_2d &b, int n, int m) {
    complex_polynomial_2d fa(n, std::vector<std::complex<float> >(m));
    complex_polynomial_2d fb(n, std::vector<std::complex<float> >(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fa[i][j] = a[i][j];
            fb[i][j] = b[i][j];
        }
    }

    fft2d(fa, false);
    fft2d(fb, false);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fa[i][j] *= fb[i][j];
        }
    }
    fft2d(fa, true);

    polynomial_2d result(n, std::vector<float>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            result[i][j] = fa[i][j].real();
        }
    }
    return result;
}

// 从多项式卷积结果中提取非零项
S2d extract_non_zero(const polynomial_2d &poly) {
    S2d result;
    for (int i = 0; i < poly.size(); i++) {
        for (int j = 0; j < poly[0].size(); j++) {
            if (poly[i][j] > 1e-6) {
                // 考虑浮点数误差
                result.emplace_back(i, j);
            }
        }
    }
    return result;
}

// 集合合并
S2d circle_add(S2d &&a, S2d &&b) {
    int n, m;
    n = a.size();
    m = b.size();
    polynomial_2d poly_a = convert_to_polynomial_2d(a, n, m);
    polynomial_2d poly_b = convert_to_polynomial_2d(b, n, m);

    polynomial_2d poly_result = polynomial_convolution_2d(poly_a, poly_b, n, m);

    return extract_non_zero(poly_result);
}

S2d convert_to_S2d(int x) { return {{0, 0}, {x, 1}}; }

S2d AllSubsetSums_sharp(const std::vector<int> &S, int u) {
    // 基准情况：如果 S 只有一个元素
    if (S.size() == 1) {
        return convert_to_S2d(S[0]);
    }

    // 将 S 分解为两个子集 T 和 S \ T
    int n = S.size();
    int half = n / 2;
    std::vector<int> T(S.begin(), S.begin() + half);
    std::vector<int> S_minus_T(S.begin() + half, S.end());

    // 递归计算
    S2d subset_sums_T = AllSubsetSums_sharp(T, u);
    S2d subset_sums_S_minus_T = AllSubsetSums_sharp(S_minus_T, u);

    // 合并结果
    return circle_add(std::move(subset_sums_T), std::move(subset_sums_S_minus_T));
}

int main() {

    freopen("data/in.in", "r", stdin);
    int n, u;
    std::cin >> n >> u;

    std::vector<int> S(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> S[i];
    }

    S2d result = AllSubsetSums_sharp(S, u);

    for (const auto &p: result) {
        std::cout << "(" << p.first << ", " << p.second << ")" << std::endl;
    }

    return 0;
}
