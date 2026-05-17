#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int popcountUnsigned(unsigned int x) {
    int cnt = 0;
    while (x != 0u) {
        x &= (x - 1u);
        ++cnt;
    }
    return cnt;
}

struct QueueNode {
    long long cost;
    int last;
    int id;
};

struct QueueNodeCompare {
    bool operator()(const QueueNode& a, const QueueNode& b) const {
        return a.cost > b.cost;
    }
};

struct Label {
    unsigned int nonMask;
    unsigned int critState;
    long long cost;
    bool active;
};

class Solver {
public:
    int n;
    int m;
    int q;
    int P;
    int nonCnt;

    vector<int> critical;
    vector<int> critIndex;
    vector<int> nonIndex;

    vector<vector<long long> > sw;
    vector<int> phase;
    vector<long long> baseCost;
    vector<unsigned int> winNonMask;
    vector<unsigned int> winCritAvail;
    vector<vector<int> > winCritList;

    vector<unsigned int> suffixNonMask;
    vector<unsigned int> suffixCritAvail;

    vector<vector<Label> > labels;
    priority_queue<QueueNode, vector<QueueNode>, QueueNodeCompare> pq;

    unsigned int fullNonMask;
    long long bestAnswer;

    Solver() {
        n = 0;
        m = 0;
        q = 0;
        P = 0;
        nonCnt = 0;
        fullNonMask = 0;
        bestAnswer = -1;
    }

    unsigned int getNibble(unsigned int mask, int idx) {
        return (mask >> (idx * 4)) & 15u;
    }

    unsigned int setNibble(unsigned int mask, int idx, unsigned int value) {
        unsigned int clearMask = ~(15u << (idx * 4));
        return (mask & clearMask) | (value << (idx * 4));
    }

    bool nibbleDominates(unsigned int a, unsigned int b) {
        if (a == b) {
            return true;
        }
        if (a == 15u) {
            return true;
        }
        if (b == 15u) {
            return false;
        }
        return ((a | b) == a);
    }

    bool critDominates(unsigned int a, unsigned int b) {
        int i;
        for (i = 0; i < q; ++i) {
            if (!nibbleDominates(getNibble(a, i), getNibble(b, i))) {
                return false;
            }
        }
        return true;
    }

    bool stateDominates(unsigned int aNon, unsigned int aCrit,
                        unsigned int bNon, unsigned int bCrit) {
        if ((aNon | bNon) != aNon) {
            return false;
        }
        if (!critDominates(aCrit, bCrit)) {
            return false;
        }
        return true;
    }

    bool goalReached(unsigned int nonMask, unsigned int critState) {
        if (nonMask != fullNonMask) {
            return false;
        }
        int i;
        for (i = 0; i < q; ++i) {
            if (getNibble(critState, i) != 15u) {
                return false;
            }
        }
        return true;
    }

    unsigned int applyWindowToCrit(unsigned int critState, int windowId) {
        unsigned int result = critState;
        unsigned int phaseBit = 1u << (phase[windowId] - 1);
        int i;
        for (i = 0; i < (int)winCritList[windowId].size(); ++i) {
            int idx = winCritList[windowId][i];
            unsigned int cur = getNibble(result, idx);
            if (cur == 15u) {
                continue;
            }
            if (cur == 0u || cur == phaseBit) {
                result = setNibble(result, idx, phaseBit);
            } else {
                result = setNibble(result, idx, 15u);
            }
        }
        return result;
    }

    bool canFinish(int last, unsigned int nonMask, unsigned int critState) {
        if ((nonMask | suffixNonMask[last + 1]) != fullNonMask) {
            return false;
        }

        int i;
        for (i = 0; i < q; ++i) {
            unsigned int cur = getNibble(critState, i);
            if (cur == 15u) {
                continue;
            }
            unsigned int future = getNibble(suffixCritAvail[last + 1], i);
            if (popcountUnsigned(cur | future) < 2) {
                return false;
            }
        }
        return true;
    }

    bool insertLabel(int last, unsigned int nonMask, unsigned int critState, long long cost) {
        if (cost > bestAnswer) {
            return false;
        }
        if (!canFinish(last, nonMask, critState)) {
            return false;
        }

        vector<Label>& bucket = labels[last];
        int i;
        for (i = 0; i < (int)bucket.size(); ++i) {
            if (!bucket[i].active) {
                continue;
            }
            if (bucket[i].cost <= cost &&
                stateDominates(bucket[i].nonMask, bucket[i].critState, nonMask, critState)) {
                return false;
            }
        }

        for (i = 0; i < (int)bucket.size(); ++i) {
            if (!bucket[i].active) {
                continue;
            }
            if (cost <= bucket[i].cost &&
                stateDominates(nonMask, critState, bucket[i].nonMask, bucket[i].critState)) {
                bucket[i].active = false;
            }
        }

        Label node;
        node.nonMask = nonMask;
        node.critState = critState;
        node.cost = cost;
        node.active = true;
        bucket.push_back(node);

        QueueNode qnode;
        qnode.cost = cost;
        qnode.last = last;
        qnode.id = (int)bucket.size() - 1;
        pq.push(qnode);

        if (goalReached(nonMask, critState) && cost < bestAnswer) {
            bestAnswer = cost;
        }
        return true;
    }

    bool preprocessPossible(const vector<vector<int> >& cover) {
        vector<int> nonSeen(m + 1, 0);
        vector<unsigned int> critPhaseUnion(q, 0u);

        int i, j;
        for (i = 1; i <= n; ++i) {
            for (j = 0; j < (int)cover[i].size(); ++j) {
                int x = cover[i][j];
                if (critIndex[x] == -1) {
                    nonSeen[x] = 1;
                } else {
                    critPhaseUnion[critIndex[x]] |= (1u << (phase[i] - 1));
                }
            }
        }

        for (i = 1; i <= m; ++i) {
            if (critIndex[i] == -1 && !nonSeen[i]) {
                return false;
            }
        }

        for (i = 0; i < q; ++i) {
            if (popcountUnsigned(critPhaseUnion[i]) < 2) {
                return false;
            }
        }
        return true;
    }

    void buildMasks(const vector<long long>& open,
                    const vector<long long>& risk,
                    const vector<vector<int> >& cover) {
        int i, j;

        baseCost.assign(n + 1, 0);
        winNonMask.assign(n + 1, 0u);
        winCritAvail.assign(n + 1, 0u);
        winCritList.assign(n + 1, vector<int>());

        for (i = 1; i <= n; ++i) {
            baseCost[i] = open[i] + risk[i];
            unsigned int phaseBit = 1u << (phase[i] - 1);

            for (j = 0; j < (int)cover[i].size(); ++j) {
                int x = cover[i][j];
                if (critIndex[x] == -1) {
                    winNonMask[i] |= (1u << nonIndex[x]);
                } else {
                    int idx = critIndex[x];
                    winCritAvail[i] |= (phaseBit << (idx * 4));
                    winCritList[i].push_back(idx);
                }
            }

            sort(winCritList[i].begin(), winCritList[i].end());
            winCritList[i].erase(unique(winCritList[i].begin(), winCritList[i].end()), winCritList[i].end());
        }
    }

    void buildSuffixInfo() {
        int i;
        suffixNonMask.assign(n + 2, 0u);
        suffixCritAvail.assign(n + 2, 0u);

        for (i = n; i >= 1; --i) {
            suffixNonMask[i] = suffixNonMask[i + 1] | winNonMask[i];

            unsigned int merged = suffixCritAvail[i + 1];
            int k;
            for (k = 0; k < q; ++k) {
                unsigned int a = getNibble(merged, k);
                unsigned int b = getNibble(winCritAvail[i], k);
                merged = setNibble(merged, k, a | b);
            }
            suffixCritAvail[i] = merged;
        }
    }

    long long initialUpperBound() {
        long long sum = 0;
        int i;
        for (i = 1; i <= n; ++i) {
            sum += baseCost[i];
        }
        for (i = 1; i < n; ++i) {
            sum += sw[i][i + 1];
        }
        return sum;
    }

    long long solve(int nInput, int mInput, int qInput, int PInput,
                    const vector<int>& criticalInput,
                    const vector<vector<long long> >& swInput,
                    const vector<int>& phaseInput,
                    const vector<long long>& openInput,
                    const vector<long long>& riskInput,
                    const vector<vector<int> >& coverInput) {
        n = nInput;
        m = mInput;
        q = qInput;
        P = PInput;

        critical = criticalInput;
        sw = swInput;
        phase = phaseInput;

        critIndex.assign(m + 1, -1);
        nonIndex.assign(m + 1, -1);

        int i;
        for (i = 0; i < q; ++i) {
            critIndex[critical[i]] = i;
        }

        nonCnt = 0;
        for (i = 1; i <= m; ++i) {
            if (critIndex[i] == -1) {
                nonIndex[i] = nonCnt;
                ++nonCnt;
            }
        }

        if (nonCnt == 0) {
            fullNonMask = 0u;
        } else {
            fullNonMask = (1u << nonCnt) - 1u;
        }

        if (!preprocessPossible(coverInput)) {
            return -1;
        }

        buildMasks(openInput, riskInput, coverInput);
        buildSuffixInfo();

        labels.clear();
        labels.resize(n + 1);
        while (!pq.empty()) {
            pq.pop();
        }

        bestAnswer = initialUpperBound();
        insertLabel(0, 0u, 0u, 0LL);

        while (!pq.empty()) {
            QueueNode cur = pq.top();
            pq.pop();

            if (cur.last < 0 || cur.last > n) {
                continue;
            }
            if (cur.id < 0 || cur.id >= (int)labels[cur.last].size()) {
                continue;
            }

            Label& now = labels[cur.last][cur.id];
            if (!now.active) {
                continue;
            }
            if (now.cost != cur.cost) {
                continue;
            }

            if (goalReached(now.nonMask, now.critState)) {
                return now.cost;
            }

            if (now.cost > bestAnswer) {
                continue;
            }

            int nxt;
            for (nxt = cur.last + 1; nxt <= n; ++nxt) {
                long long newCost = now.cost + baseCost[nxt];
                if (cur.last != 0) {
                    newCost += sw[cur.last][nxt];
                }

                unsigned int newNonMask = now.nonMask | winNonMask[nxt];
                unsigned int newCritState = applyWindowToCrit(now.critState, nxt);

                insertLabel(nxt, newNonMask, newCritState, newCost);
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

    vector<int> critical(q);
    int i, j;
    for (i = 0; i < q; ++i) {
        cin >> critical[i];
    }

    vector<vector<long long> > sw(n + 1, vector<long long>(n + 1, 0));
    for (i = 1; i <= n; ++i) {
        for (j = i + 1; j <= n; ++j) {
            cin >> sw[i][j];
        }
    }

    vector<int> phase(n + 1, 0);
    vector<long long> open(n + 1, 0), risk(n + 1, 0);
    vector<vector<int> > cover(n + 1);

    for (i = 1; i <= n; ++i) {
        int c;
        cin >> phase[i] >> open[i] >> risk[i] >> c;
        cover[i].resize(c);
        for (j = 0; j < c; ++j) {
            cin >> cover[i][j];
        }
    }

    Solver solver;
    long long ans = solver.solve(n, m, q, P, critical, sw, phase, open, risk, cover);
    cout << ans << '\n';
    return 0;
}
