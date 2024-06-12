#include <iostream>
#include <cstdio>
#include <cstring>

#define ll long long
#define MAXN 100


using std::cin, std::cout, std::endl;

int seq[MAXN];

int n, m;

bool choose[MAXN];

void dfs(int u, ll sum) {
    if (u > n) {
        if (sum != m)
            return;
        cout << "sum: " << sum << endl;
        for (int i = 1; i <= n; i++)
            cout << choose[i];
        cout << endl;
        return;
    }
    choose[u] = true;
    dfs(u + 1, sum + seq[u]);
    choose[u] = false;
    dfs(u + 1, sum);
}


int main(int argc, char *argv[]) {
    freopen("data/in.in", "r", stdin);

    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> seq[i];
        cout << seq[i] << endl;
    }
    dfs(1, 0);
    return 0;
}
