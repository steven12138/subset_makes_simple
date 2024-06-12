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


bool choice[MAXN];

void TraceBack(int x, int rest) {
    if (x == 1) {
        if (rest == seq[1]) {
            choice[1] = 1, rest = 0;
        }
        if (!rest) {
            std::cout << "sum: " << m << std::endl;
            for (int i = 1; i <= n; i++)
                if (choice[i])
                    std::cout << seq[i] << " ";
            std::cout << std::endl;
            for (int i = 1; i <= n; i++)
                std::cout << choice[i];
            std::cout << std::endl;
        }
        choice[1] = 0;
        return;
    }
    if (f[x][rest - seq[x]] == rest - seq[x]) {
        choice[x] = 1;
        TraceBack(x - 1, rest - seq[x]);
        choice[x] = 0;
    }
    if (f[x][rest] == rest) TraceBack(x - 1, rest);
}

int main(int argc, char *argv[]) {
    freopen("data/in.in", "r", stdin);

    std::cin >> n >> m;

    for (int i = 1; i <= n; i++) {
        std::cin >> seq[i];
    }

    Backpack();
    TraceBack(n, m);
    return 0;
}
