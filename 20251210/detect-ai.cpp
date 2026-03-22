#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <fstream>

using namespace std;

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <unordered_map>
#include <climits>
#include <fstream>
using namespace std;

// #define log
#ifdef log
#include <chrono>
#endif
#include <numeric>

class Solution {
public:
    int maxWalls(vector<int>& cameras_vec, vector<int>& detection_range_vec, vector<int>& obs) {
#ifdef log
        auto start = std::chrono::high_resolution_clock::now();
#endif
        int n = cameras_vec.size();
        int m = obs.size();
        vector<pair<int, int>> cam(n); // (position, range)

        sort(obs.begin(), obs.end());

        for (int i = 0; i < n; ++i) {
            cam[i] = {cameras_vec[i], detection_range_vec[i]};
        }

        sort(cam.begin(), cam.end());

        vector<int> pos(n), rang(n);
        for (int i = 0; i < n; ++i) {
            pos[i] = cam[i].first;
            rang[i] = cam[i].second;
        }

    
        // Step 1: Count obstacles exactly at camera positions (fixed_count)
        int fixed_count = 0;
        vector<int> new_obs;
        int j = 0;
        for (int i = 0; i < m; ++i) {
            while (j < n && pos[j] < obs[i]) j++;
            if (j < n && pos[j] == obs[i]) {
                fixed_count++;
                // Skip this obstacle (it will be covered by the camera itself)
            } else {
                new_obs.push_back(obs[i]);
            }
        }
        int M = new_obs.size();
        
        // Step 2: Assign remaining obstacles to segments
        // segments[0]: (-inf, pos[0]), segments[i] for 1<=i<=n-1: (pos[i-1], pos[i]), segments[n]: (pos[n-1], +inf)
        vector<vector<int>> segments(n + 1);
        for (int i = 0; i < M; ++i) {
            int p = new_obs[i];
            int idx = upper_bound(pos.begin(), pos.end(), p) - pos.begin();
            segments[idx].push_back(p);
        }
        
        // Step 3: Precompute values for each segment
        vector<int> left_count(n + 1, 0), right_count(n + 1, 0), a(n + 1, 0), b(n + 1, 0), u(n + 1, 0);
        
        // Segment 0
        if (!segments[0].empty()) {
            int L0 = pos[0] - rang[0];
            auto &vec = segments[0];
            int idx = lower_bound(vec.begin(), vec.end(), L0) - vec.begin();
            left_count[0] = vec.size() - idx;
        }
        
        // Segment n
        if (!segments[n].empty()) {
            int Rn = pos[n - 1] + rang[n - 1];
            auto &vec = segments[n];
            int idx = upper_bound(vec.begin(), vec.end(), Rn) - vec.begin();
            right_count[n] = idx;
        }
        
        // Segments 1 to n-1
        for (int seg = 1; seg <= n - 1; ++seg) {
            if (segments[seg].empty()) continue;
            int R = min(pos[seg - 1] + rang[seg - 1], pos[seg] - 1);
            int L = max(pos[seg] - rang[seg], pos[seg - 1] + 1);
            auto &vec = segments[seg];
            
            int idx_r = upper_bound(vec.begin(), vec.end(), R) - vec.begin();
            a[seg] = idx_r;
            
            int idx_l = lower_bound(vec.begin(), vec.end(), L) - vec.begin();
            b[seg] = vec.size() - idx_l;
            
            int c = max(0, idx_r - idx_l);
            u[seg] = a[seg] + b[seg] - c;
        }
        
        // Step 4: Dynamic programming to choose directions
        if (n == 1) {
            // Only one camera
            int dp0_0 = left_count[0]; // choose left
            int dp0_1 = 0;              // choose right
            int total0 = dp0_0 + 0;      // segment n=1: cover_n(0)=0
            int total1 = dp0_1 + right_count[1]; // cover_n(1)=right_count[1]
            int max_seg = max(total0, total1);
            int ans = fixed_count + max_seg;
            // cout << ans << endl;
            return ans;
        }
        
        // n >= 2
        vector<vector<int>> dp(n, vector<int>(2, 0));
        dp[0][0] = left_count[0];
        dp[0][1] = 0;
        
        for (int i = 1; i <= n - 1; ++i) {
            for (int s = 0; s <= 1; ++s) {
                int best = -1;
                for (int t = 0; t <= 1; ++t) {
                    int val = dp[i - 1][t];
                    int cover = 0;
                    if (t == 1 && s == 0) cover = u[i];
                    else if (t == 1 && s == 1) cover = a[i];
                    else if (t == 0 && s == 0) cover = b[i];
                    else cover = 0; // (t==0 && s==1)
                    val += cover;
                    if (val > best) best = val;
                }
                dp[i][s] = best;
            }
        }
        
        // Add contribution from segment n
        int max_seg = 0;
        for (int s = 0; s <= 1; ++s) {
            int val = dp[n - 1][s];
            if (s == 1) val += right_count[n];
            if (val > max_seg) max_seg = val;
        }
        
        int ans = fixed_count + max_seg;

// #ifdef log
//         // 获取结束时间点
//         auto end = std::chrono::high_resolution_clock::now();
//         // 计算持续时间（以毫秒为单位）
//         auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//         auto duration_mid0 = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start).count();
//         auto duration_mid = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid).count();
//         std::cout << "函数执行时间: " << duration_seconds << " 毫秒" << std::endl;
//         std::cout << "阶段1执行时间: " << duration_mid0 << " 毫秒" << std::endl;
//         std::cout << "阶段2执行时间: " << duration_mid << " 毫秒" << std::endl;
// #endif
        return ans;
    }
};


#define INPUT_FROM_FILE 1

int main(int argc, char* argv[]) {

    // 默认从标准输入读取
    istream* input_stream = &cin;
    ifstream file_stream;
    
    // 如果定义了INPUT_FROM_FILE宏，则尝试从文件读取
#if INPUT_FROM_FILE
    if (argc > 1) {
        file_stream.open(argv[1]);
        if (file_stream.is_open()) {
            input_stream = &file_stream;
        } else {
            cerr << "无法打开文件: " << argv[1] << endl;
            return 1;
        }
    } else {
        cerr << "请提供输入文件路径" << endl;
        return 1;
    }
#endif
    int n, m;
    *input_stream >> n >> m;
    
    // vector<pair<int, int>> cam(n); // (position, range)
    vector<int> cameras_vec(n);
    vector<int> detection_range_vec(n);
    vector<int> obstacles_vec(m);

    for (int i = 0; i < n; ++i) {
        *input_stream >> cameras_vec[i];
    }
    for (int i = 0; i < n; ++i) {
        *input_stream >> detection_range_vec[i];
    }
    
    // Sort cameras by position
    for (int i = 0; i < m; ++i) {
        *input_stream >> obstacles_vec[i];
    }

    Solution solution;

    int total_obstacles_covered = solution.maxWalls(cameras_vec, detection_range_vec, obstacles_vec);

    cout << total_obstacles_covered << endl;
    
    return 0;
}