#include <functional>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n, health;
    if (!(cin >> m >> n >> health)) return 0;
    vector<vector<int>> grid(m, vector<int>(n));
    for (auto &row : grid) {
        for (int &cell : row) cin >> cell;
    }

    const int INF = 1e9;
    vector<vector<int>> dist(m, vector<int>(n, INF));
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;
    dist[0][0] = grid[0][0];
    pq.emplace(dist[0][0], 0, 0);
    const int dr[] = {1, -1, 0, 0};
    const int dc[] = {0, 0, 1, -1};
    while (!pq.empty()) {
        int cost = get<0>(pq.top());
        int r = get<1>(pq.top());
        int c = get<2>(pq.top());
        pq.pop();
        if (cost != dist[r][c]) continue;
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;
            int nextCost = cost + grid[nr][nc];
            if (nextCost < dist[nr][nc]) {
                dist[nr][nc] = nextCost;
                pq.emplace(nextCost, nr, nc);
            }
        }
    }
    cout << (dist[m - 1][n - 1] < health ? 1 : 0) << '\n';
    return 0;
}
