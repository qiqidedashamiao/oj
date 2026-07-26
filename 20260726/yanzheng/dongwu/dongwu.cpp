#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<int> pos(m + 2);
    pos[0] = 0;
    pos[m + 1] = n + 1;
    for (int i = 1; i <= m; ++i) cin >> pos[i];
    sort(pos.begin() + 1, pos.begin() + m + 1);

    vector<vector<int>> dp(m + 2, vector<int>(m + 2));
    for (int len = 1; len <= m; ++len) {
        for (int l = 1; l + len - 1 <= m; ++l) {
            int r = l + len - 1;
            dp[l][r] = 1e9;
            int cost = pos[r + 1] - pos[l - 1] - 2;
            for (int k = l; k <= r; ++k) {
                dp[l][r] = min(dp[l][r], dp[l][k - 1] + dp[k + 1][r] + cost);
            }
        }
    }
    cout << dp[1][m] << '\n';
    return 0;
}
