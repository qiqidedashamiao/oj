#include <bits/stdc++.h>
using namespace std;

struct State {
    long long cost;
    int last;
    int mask;
    uint32_t key_phase;
};

struct ComparePQ {
    bool operator()(const pair<long long, State>& a, const pair<long long, State>& b) const {
        return a.first > b.first;
    }
};

class Solution {
public:
    int minCost(int n, int m, int q, int P,
                vector<int>& critical,
                vector<vector<long long>>& sw,
                vector<int>& phase,
                vector<int>& open,
                vector<int>& risk,
                vector<vector<int>>& cover) {
        vector<int> is_crit(m + 1, -1);
        for (int i = 0; i < q; ++i) {
            is_crit[critical[i]] = i;
        }

        const long long INF = 1e18;
        int full_mask = (1 << m) - 1;

        vector<int> cov_mask(n + 1, 0);
        vector<uint32_t> phase_contrib(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            int mask = 0;
            uint32_t pc = 0;
            int p = phase[i] - 1;
            for (int x : cover[i]) {
                mask |= (1 << (x - 1));
                if (is_crit[x] != -1) {
                    int idx = is_crit[x];
                    pc |= (1 << (4 * idx + p));
                }
            }
            cov_mask[i] = mask;
            phase_contrib[i] = pc;
        }

        vector<long long> min_cost(m + 1, INF);
        for (int i = 1; i <= n; ++i) {
            long long c = open[i] + risk[i];
            for (int x : cover[i]) {
                if (c < min_cost[x]) min_cost[x] = c;
            }
        }
        for (int x = 1; x <= m; ++x) {
            if (min_cost[x] == INF) return -1;
        }

        vector<vector<long long>> extra(q, vector<long long>(P, INF));
        vector<long long> min_two(q, INF);
        vector<vector<long long>> best_phase(q, vector<long long>(P, INF));
        for (int i = 1; i <= n; ++i) {
            long long c = open[i] + risk[i];
            int p = phase[i] - 1;
            for (int x : cover[i]) {
                int idx = is_crit[x];
                if (idx == -1) continue;
                if (c < best_phase[idx][p]) {
                    best_phase[idx][p] = c;
                }
            }
        }
        for (int k = 0; k < q; ++k) {
            for (int p = 0; p < P; ++p) {
                long long best_other = INF;
                for (int p2 = 0; p2 < P; ++p2) {
                    if (p2 != p && best_phase[k][p2] < best_other) {
                        best_other = best_phase[k][p2];
                    }
                }
                extra[k][p] = best_other;
            }
            vector<long long> costs;
            for (int p = 0; p < P; ++p) {
                if (best_phase[k][p] < INF) costs.push_back(best_phase[k][p]);
            }
            sort(costs.begin(), costs.end());
            if (costs.size() >= 2) {
                min_two[k] = costs[0] + costs[1];
            }
        }

        auto heuristic = [&](int mask, uint32_t key_phase) -> long long {
            long long h = 0;
            int need = full_mask ^ mask;
            for (int x = 1; x <= m; ++x) {
                if (need & (1 << (x - 1))) {
                    int k = is_crit[x];
                    if (k != -1) {
                        if (min_two[k] < INF) h += min_two[k];
                        else h += min_cost[x] * 2;
                    } else {
                        h += min_cost[x];
                    }
                }
            }
            for (int k = 0; k < q; ++k) {
                if (mask & (1 << (critical[k] - 1))) {
                    uint32_t phases = (key_phase >> (4 * k)) & 0xF;
                    if (__builtin_popcount(phases) == 1) {
                        int p = __builtin_ctz(phases);
                        long long add = extra[k][p];
                        if (add < INF) h += add;
                        else h += min_cost[critical[k]];
                    }
                }
            }
            return h;
        };

        vector<unordered_map<uint64_t, long long>> dist(n + 1);
        auto encode = [&](int mask, uint32_t kp) -> uint64_t {
            return ((uint64_t)kp << m) | mask;
        };

        auto key_satisfied = [&](uint32_t kp) -> bool {
            for (int k = 0; k < q; ++k) {
                if (__builtin_popcount((kp >> (4 * k)) & 0xF) < 2) return false;
            }
            return true;
        };

        priority_queue<pair<long long, State>, vector<pair<long long, State>>, ComparePQ> pq;
        
        auto push_state = [&](int last, int mask, uint32_t kp, long long cost) {
            if (last >= 0 && last <= n) {
                uint64_t key = encode(mask, kp);
                auto it = dist[last].find(key);
                if (it == dist[last].end() || cost < it->second) {
                    dist[last][key] = cost;
                    long long f = cost + heuristic(mask, kp);
                    pq.push(make_pair(f, State{cost, last, mask, kp}));
                }
            }
        };

        push_state(0, 0, 0, 0);

        while (!pq.empty()) {
            pair<long long, State> top = pq.top(); pq.pop();
            State state = top.second;
            long long cost = state.cost;
            int last = state.last;
            int mask = state.mask;
            uint32_t kp = state.key_phase;

            uint64_t key = encode(mask, kp);
            if (cost > dist[last][key]) continue;

            if (mask == full_mask && key_satisfied(kp)) {
                return (int)cost;
            }

            for (int nxt = last + 1; nxt <= n; ++nxt) {
                long long new_cost = cost + open[nxt] + risk[nxt];
                if (last != 0) new_cost += sw[last][nxt];
                int new_mask = mask | cov_mask[nxt];
                uint32_t new_kp = kp | phase_contrib[nxt];
                push_state(nxt, new_mask, new_kp, new_cost);
            }
        }
        return -1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q, P;
    cin >> n >> m >> q >> P;

    vector<int> critical;
    if (q > 0) {
        critical.resize(q);
        for (int i = 0; i < q; ++i) cin >> critical[i];
    }

    // 切换代价矩阵，下标从1开始
    vector<vector<long long>> sw(n + 1, vector<long long>(n + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            cin >> sw[i][j];
        }
    }

    vector<int> phase(n + 1), open(n + 1), risk(n + 1);
    vector<vector<int>> cover(n + 1);

    for (int i = 1; i <= n; ++i) {
        int c;
        cin >> phase[i] >> open[i] >> risk[i] >> c;
        cover[i].resize(c);
        for (int j = 0; j < c; ++j) {
            cin >> cover[i][j];
        }
    }

    Solution sol;
    int ans = sol.minCost(n, m, q, P, critical, sw, phase, open, risk, cover);
    cout << ans << endl;

    return 0;
}