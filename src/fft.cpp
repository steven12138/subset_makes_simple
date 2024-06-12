#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>

using namespace std;
using cd = complex<double>;
const double PI = acos(-1);

void fft(vector<cd> &a, bool invert) {
    int n = a.size();
    if (n == 1) return;

    vector<cd> a0(n / 2), a1(n / 2);
    for (int i = 0; 2 * i < n; i++) {
        a0[i] = a[i * 2];
        a1[i] = a[i * 2 + 1];
    }
    fft(a0, invert);
    fft(a1, invert);

    double ang = 2 * PI / n * (invert ? -1 : 1);
    cd w(1), wn(cos(ang), sin(ang));
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

vector<cd> multiply(const vector<cd> &a, const vector<cd> &b) {
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int n = 1;
    while (n < a.size() + b.size())
        n <<= 1;
    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];
    fft(fa, true);

    return fa;
}

vector<int> computeXorU(const vector<int> &S, const vector<int> &T, int u) {
    vector<cd> fS(u + 1), fT(u + 1);

    // Build the characteristic polynomials
    for (int s: S) {
        if (s <= u) fS[s] = 1;
    }
    for (int t: T) {
        if (t <= u) fT[t] = 1;
    }

    // Multiply the polynomials using FFT
    vector<cd> g = multiply(fS, fT);

    // Extract the result
    vector<int> result;
    for (int i = 0; i <= u; ++i) {
        if (round(g[i].real()) > 0) {
            result.push_back(i);
        }
    }

    return result;
}

vector<int> computeMultipleXorU(const vector<vector<int> > &sets, int u) {
    if (sets.empty()) return {};

    vector<int> result = sets[0];
    for (size_t i = 1; i < sets.size(); ++i) {
        result = computeXorU(result, sets[i], u);
    }

    return result;
}


using S2d = vector<pair<int, int> >;
// 计算二维集合 S ⊕u T
S2d computeXorU(const S2d& S, const S2d& T, int u, int v) {
    int max_u = max(u, v);
    int n = 1;
    while (n < 2 * max_u) n <<= 1;

    vector<cd> fS_x(n, 0), fT_x(n, 0);
    vector<cd> fS_y(n, 0), fT_y(n, 0);

    // 构造一维多项式
    for (const auto& p : S) {
        fS_x[p.first] += 1;
        fS_y[p.second] += 1;
    }
    for (const auto& p : T) {
        fT_x[p.first] += 1;
        fT_y[p.second] += 1;
    }

    // 计算乘积
    vector<cd> g_x = multiply(fS_x, fT_x);
    vector<cd> g_y = multiply(fS_y, fT_y);

    S2d result;
    for (int i = 0; i < 2 * u; ++i) {
        for (int j = 0; j < 2 * v; ++j) {
            if (round(g_x[i].real()) > 0 && round(g_y[j].real()) > 0) {
                result.emplace_back(i, j);
            }
        }
    }

    return result;
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
    return computeXorU(subset_sums_T, subset_sums_S_minus_T, u, u);
}


vector<int> AllSubsetSums(const vector<int> &S, int u) {
    int n = S.size();
    int b = floor(sqrt(n * log(n)));
    vector<vector<int> > R(b);

    for (int ell = 0; ell < b; ++ell) {
        vector<int> S_ell;
        for (int x: S) {
            if (x % b == ell) {
                S_ell.push_back(x);
            }
        }

        vector<int> Q_ell;
        for (int x: S_ell) {
            Q_ell.push_back((x - ell) / b);
        }

        S2d S_u_div_b_sharp_Q_ell = AllSubsetSums_sharp(Q_ell, u / b);

        for (const auto &p: S_u_div_b_sharp_Q_ell) {
            int z = p.first;
            int j = p.second;
            R[ell].push_back(z * b + j * ell);
        }
    }

    // Combine all R_ell sets using the provided logic
    vector<int> result = R[0];
    for (int i = 1; i < b; ++i) {
        result = computeXorU(result, R[i], u);
    }

    return result;
}


int main() {
    freopen("data/in.in", "r", stdin);
    int n, u;
    std::cin >> n >> u;

    std::vector<int> S(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> S[i];
    }

    vector<int> result = AllSubsetSums(S, u);

    // for (const auto &p: result) {
    //     std::cout << p << std::endl;
    // }

    return 0;
}
