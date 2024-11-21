#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

const int MOD = 1000000007;

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    unordered_map<int, long long> count;
    long long result = 0;

    for (int num : nums) {
        result = (result + num) % MOD;
        if (count.find(num - k) != count.end()) {
            result = (result + count[num - k] * num) % MOD;
        }
        if (count.find(num + k) != count.end()) {
            result = (result + count[num + k] * num) % MOD;
        }
        count[num] = (count[num] + num) % MOD;
    }

    cout << result << endl;
    return 0;
}