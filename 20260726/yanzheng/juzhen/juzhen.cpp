#include <iostream>
#include <vector>

using namespace std;

static const long long MOD = 1000000007LL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    vector<int> ns(t);
    int maxN = 0;
    for (int &n : ns) {
        cin >> n;
        maxN = max(maxN, n * n);
    }

    vector<long long> fact(maxN + 1), invFact(maxN + 1);
    fact[0] = 1;
    for (int i = 1; i <= maxN; ++i) fact[i] = fact[i - 1] * i % MOD;
    auto power = [](long long a, long long e) {
        long long result = 1;
        while (e) {
            if (e & 1) result = result * a % MOD;
            a = a * a % MOD;
            e >>= 1;
        }
        return result;
    };
    invFact[maxN] = power(fact[maxN], MOD - 2);
    for (int i = maxN; i > 0; --i) invFact[i - 1] = invFact[i] * i % MOD;

    for (int n : ns) {
        long long cells = 1LL * n * n;
        long long sum = 0;
        for (int value = 1; value <= n; ++value) {
            // Fix value as a row minimum, then arrange its row and all other cells.
            long long waysForRow = fact[cells - value] * invFact[cells - value - n + 1] % MOD;
            sum = (sum + waysForRow) % MOD;
        }
        long long answer = cells % MOD * fact[cells - n] % MOD * sum % MOD;
        cout << answer << '\n';
    }
    return 0;
}
