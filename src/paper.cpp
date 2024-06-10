#include <cmath>
#include <complex>
#include <iostream>
#include <utility>
#include <vector>

#define MAXN 105

using namespace std;

typedef vector<double> polynomial;
typedef vector<complex<double>> complex_polynomial;

const double PI = acos(-1);

void fft(complex_polynomial &a, bool invert) {
  int n = a.size();
  if (n == 1)
    return;

  // 将序列按奇偶分开
  complex_polynomial a0(n / 2), a1(n / 2);
  for (int i = 0; 2 * i < n; i++) {
    a0[i] = a[i * 2];
    a1[i] = a[i * 2 + 1];
  }
  fft(a0, invert);
  fft(a1, invert);

  // 计算单位根
  double ang = 2 * PI / n * (invert ? -1 : 1);
  complex<double> w(1), wn(cos(ang), sin(ang));
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

polynomial polynomial_convolution(const polynomial &a, const polynomial &b) {
  complex_polynomial fa(a.begin(), a.end()), fb(b.begin(), b.end());
  int n = 1;
  while (n < a.size() + b.size())
    n <<= 1;
  fa.resize(n);
  fb.resize(n);

  fft(fa, false);
  fft(fb, false);
  for (int i = 0; i < n; i++) {
    fa[i] *= fb[i];
  }
  fft(fa, true);

  polynomial result(n);
  for (int i = 0; i < n; i++) {
    result[i] = fa[i].real();
  }
  return result;
}

int n, m;
int a[MAXN];

typedef vector<pair<int, int>> S2d;

S2d all_subset_sum_sharp(int l, int r) { return {}; }

int main() {
  polynomial a = {1, 2, 3}; // 多项式A的系数
  polynomial b = {4, 5, 6}; // 多项式B的系数

  polynomial result = polynomial_convolution(a, b);

  cout << "卷积结果：";
  for (double x : result) {
    cout << x << " ";
  }
  cout << endl;

  return 0;
}
