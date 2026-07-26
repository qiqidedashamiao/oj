#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

using namespace std;

using int64 = long long;

int64 solveTree(int u, int parent, const vector<vector<int>>& graph, const vector<int64>& weight,
                vector<array<int64, 2>>& dp, const vector<char>& inCycle) {
    dp[u][0] = 0;
    dp[u][1] = weight[u];
    for (int v : graph[u]) {
        if (v == parent || inCycle[v]) continue;
        solveTree(v, u, graph, weight, dp, inCycle);
        dp[u][0] += max(dp[v][0], dp[v][1]);
        dp[u][1] += dp[v][0];
    }
    return max(dp[u][0], dp[u][1]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<int64> weight(n);
    vector<int> enemy(n);
    vector<vector<int>> graph(n);
    for (int i = 0; i < n; ++i) {
        cin >> weight[i] >> enemy[i];
        --enemy[i];
        graph[i].push_back(enemy[i]);
        if (enemy[i] != i) graph[enemy[i]].push_back(i);
    }

    vector<int> state(n), stackIndex(n, -1);
    vector<vector<int>> cycles;
    vector<int> path;
    auto dfsCycle = [&](auto&& self, int u) -> void {
        state[u] = 1;
        stackIndex[u] = static_cast<int>(path.size());
        path.push_back(u);
        int v = enemy[u];
        if (state[v] == 0) self(self, v);
        else if (state[v] == 1) cycles.emplace_back(path.begin() + stackIndex[v], path.end());
        path.pop_back();
        stackIndex[u] = -1;
        state[u] = 2;
    };
    for (int i = 0; i < n; ++i) if (state[i] == 0) dfsCycle(dfsCycle, i);

    vector<char> inCycle(n);
    for (const auto& cycle : cycles) for (int u : cycle) inCycle[u] = 1;
    vector<array<int64, 2>> dp(n);
    int64 answer = 0;
    for (const auto& cycle : cycles) {
        for (int u : cycle) solveTree(u, -1, graph, weight, dp, inCycle);
        int k = static_cast<int>(cycle.size());
        if (k == 1) {
            answer += dp[cycle[0]][0];
            continue;
        }
        const int64 NEG = -(1LL << 60);
        int64 prev0 = dp[cycle[0]][0], prev1 = NEG;
        for (int i = 1; i < k; ++i) {
            int64 next0 = max(prev0, prev1) + dp[cycle[i]][0];
            int64 next1 = prev0 + dp[cycle[i]][1];
            prev0 = next0;
            prev1 = next1;
        }
        int64 withoutFirst = max(prev0, prev1);
        prev0 = NEG;
        prev1 = dp[cycle[0]][1];
        for (int i = 1; i < k; ++i) {
            int64 next0 = max(prev0, prev1) + dp[cycle[i]][0];
            int64 next1 = prev0 == NEG ? NEG : prev0 + dp[cycle[i]][1];
            prev0 = next0;
            prev1 = next1;
        }
        answer += max(withoutFirst, prev0);
    }
    cout << answer << '\n';
    return 0;
}
