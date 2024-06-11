#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <set>

using namespace std;

using namespace std;
using cd = complex<double>;
const double PI = acos(-1);

// 快速傅里叶变换（FFT）
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

// 多项式乘法
vector<int> multiply(const vector<int> &a, const vector<int> &b) {
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    size_t n = 1;
    while (n < a.size() + b.size()) n <<= 1;
    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);
    for (size_t i = 0; i < n; i++) {
        fa[i] *= fb[i];
    }
    fft(fa, true);

    vector<int> result(n);
    for (size_t i = 0; i < n; i++) {
        result[i] = round(fa[i].real());
    }
    return result;
}

vector<pair<int, int> > pointwise_pairwise_sums(const vector<pair<int, int> > &S, const vector<pair<int, int> > &T,
                                                int u, int v) {
    vector<vector<int> > fS(u + 1, vector<int>(v + 1, 0)), fT(u + 1, vector<int>(v + 1, 0));
    for (auto p: S) {
        if (p.first <= u && p.second <= v) {
            fS[p.first][p.second] = 1;
        }
    }
    for (auto p: T) {
        if (p.first <= u && p.second <= v) {
            fT[p.first][p.second] = 1;
        }
    }

    vector<int> flatS((u + 1) * (v + 1), 0), flatT((u + 1) * (v + 1), 0);
    for (int i = 0; i <= u; i++) {
        for (int j = 0; j <= v; j++) {
            flatS[i * (v + 1) + j] = fS[i][j];
            flatT[i * (v + 1) + j] = fT[i][j];
        }
    }

    vector<int> result_poly = multiply(flatS, flatT);
    vector<pair<int, int> > result;
    for (int i = 0; i <= u; i++) {
        for (int j = 0; j <= v; j++) {
            if (result_poly[i * (v + 1) + j] > 0) {
                result.push_back({i, j});
            }
        }
    }
    return result;
}

vector<int> subset_pairwise_sums(const vector<int> &S, const vector<int> &T, int u) {
    vector<int> fS(u + 1, 0), fT(u + 1, 0);
    for (int x: S) {
        if (x <= u) {
            fS[x] = 1;
        }
    }
    for (int x: T) {
        if (x <= u) {
            fT[x] = 1;
        }
    }

    vector<int> result_poly = multiply(fS, fT);
    vector<int> result;
    for (int i = 0; i <= u; i++) {
        if (result_poly[i] > 0) {
            result.push_back(i);
        }
    }
    return result;
}

void test_subset_pairwise_sums() {
    vector<int> S = {1, 3, 4};
    vector<int> T = {2, 3, 5};
    int u = 10;

    vector<int> expected_result = {3, 4, 5, 6, 7, 8, 9}; // 手工计算得到的期望结果
    vector<int> result = subset_pairwise_sums(S, T, u);

    cout << "subset_pairwise_sums result: ";
    for (int x: result) {
        cout << x << " ";
    }
    cout << endl;

    // 验证结果
    if (result.size() != expected_result.size() || !equal(result.begin(), result.end(), expected_result.begin())) {
        cout << "Test failed!" << endl;
    } else {
        cout << "Test passed!" << endl;
    }
}

// 测试 pointwise_pairwise_sums 函数
void test_pointwise_pairwise_sums() {
    vector<pair<int, int> > S = {{1, 2}, {2, 3}, {3, 4}};
    vector<pair<int, int> > T = {{1, 1}, {2, 2}, {3, 3}};
    int u = 5;
    int v = 5;

    vector<pair<int, int> > expected_result = {{2, 3}, {3, 4}, {4, 5}, {4, 6}, {5, 5}, {5, 7}, {6, 6}, {6, 8}};
    vector<pair<int, int> > result = pointwise_pairwise_sums(S, T, u, v);

    cout << "pointwise_pairwise_sums result: ";
    for (auto p: result) {
        cout << "(" << p.first << ", " << p.second << ") ";
    }
    cout << endl;

    // 将结果转换为集合以便于比较
    set<pair<int, int> > result_set(result.begin(), result.end());
    set<pair<int, int> > expected_set(expected_result.begin(), expected_result.end());

    // 验证结果
    if (result_set != expected_set) {
        cout << "Test failed!" << endl;
    } else {
        cout << "Test passed!" << endl;
    }
}

int main() {
    // 测试 subset_pairwise_sums
    test_subset_pairwise_sums();

    // 测试 pointwise_pairwise_sums
    test_pointwise_pairwise_sums();

    return 0;
}
