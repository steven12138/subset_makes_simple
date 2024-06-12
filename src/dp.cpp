#include <iostream>
#include <algorithm>
#include <cstring>

#define MAXN 100

int seq[MAXN];

int n, m;

int f[MAXN][2463100];

void Backpack() {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            f[i][j] = f[i - 1][j];
            if (j >= seq[i])
                f[i][j] = std::max(f[i][j], f[i - 1][j - seq[i]] + seq[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    freopen("data/in.in", "r", stdin);

    std::cin >> n >> m;

    for (int i = 1; i <= n; i++) {
        std::cin >> seq[i];
    }

    Backpack();
    return 0;
}
