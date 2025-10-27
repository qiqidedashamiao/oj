#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

using namespace std;
typedef long long LL;
const LL MAX_POS = 1000000000;

void updateInterval(pair<LL, LL>& interval, LL l, LL r) {
    if (interval.first > interval.second) {
        interval = {l, r};
    } else {
        interval.first = min(interval.first, l);
        interval.second = max(interval.second, r);
    }
}

void intersectInterval(pair<LL, LL>& interval, LL l, LL r) {
    if (interval.first > interval.second) return;
    interval.first = max(interval.first, l);
    interval.second = min(interval.second, r);
    if (interval.first > interval.second) {
        interval = {1, 0};
    }
}

int n;
vector<LL> t;
vector<LL> c;

bool check(LL v) {
    vector<pair<LL, LL>> dp(n+1), ep(n+1);
    dp[1] = {1, MAX_POS};
    ep[1] = {1, MAX_POS};

    for (int i = 2; i <= n; i++) {
        dp[i] = {MAX_POS+1, 0};
        ep[i] = {MAX_POS+1, 0};

        LL time_gap = t[i] - t[i-1];

        if (llabs(c[i] - c[i-1]) <= v * time_gap) {
            LL l1 = dp[i-1].first - v * time_gap;
            LL r1 = dp[i-1].second + v * time_gap;
            updateInterval(dp[i], l1, r1);
        }

        LL L = c[i] - v * time_gap;
        LL R = c[i] + v * time_gap;
        if (ep[i-1].first <= R && ep[i-1].second >= L) {
            LL l2 = c[i-1] - v * time_gap;
            LL r2 = c[i-1] + v * time_gap;
            updateInterval(dp[i], l2, r2);
        }

        if (llabs(c[i] - c[i-1]) <= v * time_gap) {
            LL l1 = ep[i-1].first - v * time_gap;
            LL r1 = ep[i-1].second + v * time_gap;
            updateInterval(ep[i], l1, r1);
        }

        if (dp[i-1].first <= R && dp[i-1].second >= L) {
            LL l2 = c[i-1] - v * time_gap;
            LL r2 = c[i-1] + v * time_gap;
            updateInterval(ep[i], l2, r2);
        }

        intersectInterval(dp[i], 1, MAX_POS);
        intersectInterval(ep[i], 1, MAX_POS);
    }

    return (dp[n].first <= dp[n].second) || (ep[n].first <= ep[n].second);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> n;
    t.resize(n+1);
    c.resize(n+1);
    for (int i = 1; i <= n; i++) {
        cin >> t[i] >> c[i];
    }

    LL left = 0, right = 2e9;
    LL ans = -1;
    while (left <= right) {
        LL mid = left + (right - left) / 2;
        if (check(mid)) {
            ans = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    if (ans != -1) {
        cout << ans << endl;
    } else {
        cout << -1 << endl;
    }

    return 0;
}