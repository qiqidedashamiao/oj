#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <unordered_map>
#include <climits>
#include <fstream>
using namespace std;

#define log
#ifdef log
#include <chrono>
#endif
#include <numeric>

class Solution {
public:
    int maxWalls(vector<int>& cameras_vec, vector<int>& detection_range_vec, vector<int>& obstacles_vec) {
#ifdef log
        auto start = std::chrono::high_resolution_clock::now();
#endif
        int n = cameras_vec.size();
        int m = obstacles_vec.size();

        // 对obstacles_vec进行从小到大排序
        sort(obstacles_vec.begin(), obstacles_vec.end());
#ifdef log
        auto start1 = std::chrono::high_resolution_clock::now();
        auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(start1 - start).count();
        std::cout << "time1执行时间: " << time1 << " 毫秒" << std::endl;
#endif
        map<int, int> obstacles_map;

#ifdef log
        auto start2 = std::chrono::high_resolution_clock::now();
        
        auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(start2 - start1).count();
        std::cout << "time2执行时间: " << time2 << " 毫秒" << std::endl;
#endif
        vector<int> indices(cameras_vec.size());
        iota(indices.begin(), indices.end(), 0);  // 填充 0, 1, 2, ...
    
    // 根据 cameras_vec 的值对索引进行排序
        sort(indices.begin(), indices.end(), 
         [&cameras_vec](int i, int j) { 
             return cameras_vec[i] < cameras_vec[j]; 
         });

#ifdef log
        // 获取中间时间点
        auto mid = std::chrono::high_resolution_clock::now();
        auto time3 = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start2).count();
        std::cout << "time3执行时间: " << time3 << " 毫秒" << std::endl;
        // 计算持续时间（以毫秒为单位）
#endif


        // for (int i = 0; i < n; i++) {
        //     cout << i << " ";
        // }
        // cout << endl;

        // for (int i = 0; i < n; i++) {
        //     cout << cameras_vec[indices[i]] << " ";
        // }
        // cout << endl;
        // // for (int i = 0; i < n; i++) {
        // //     cout << detection_range_vec[indices[i]] << " ";
        // // }
        // // cout << endl;
        // for (int i = 0; i < n; i++) {
        //     cout << detection_range_vec[indices[i]] << " ";
        // }
        // cout << endl;
        // for (int i = 0; i < m; i++) {
        //     cout << obstacles_vec[i] << " ";
        // }
        // cout << endl;

        int index[3] = {0, 0, 0};
        vector<vector<int>> f(2, vector<int>(2, 0));

        int left = cameras_vec[indices[0]] - detection_range_vec[indices[0]];
        int right = cameras_vec[indices[0]] + detection_range_vec[indices[0]];
        if (n > 1 && right >= cameras_vec[indices[1]])
        {
            right = cameras_vec[indices[1]]-1;
        }
        int g = 0;
        for (g = index[0]; g < m; g++)
        {
            if (obstacles_vec[g] < left)
            {
                continue;
            }
            if (obstacles_vec[g] > cameras_vec[indices[0]])
            {
                break;
            }
            f[0][0]++;
        }
        index[0] = index[1] = g;

        for (g = index[2]; g < m; g++)
        {
            if (obstacles_vec[g] < cameras_vec[indices[0]])
            {
                continue;
            }
            if (obstacles_vec[g] > right)
            {
                break;
            }
            f[0][1]++;
        }
        index[2] = g;
        // cout << "i=" << 0 << " " << f[0][0] << " " << f[0][1] << endl;
        for (int i = 1; i < n; i++)
        {
            // start[0]表示当前摄像头左侧独有覆盖区间，start[1]表示当前摄像头与前一摄像头重叠覆盖区间，start[2]表示当前摄像头右侧独有覆盖区间
            int start[3] = {0,0,cameras_vec[indices[i]]};
            int end[3] = {cameras_vec[indices[i]],cameras_vec[indices[i]],0};
            start[0] = cameras_vec[indices[i]] - detection_range_vec[indices[i]];
            if (start[0] <= cameras_vec[indices[i-1]])
            {
                start[0] = cameras_vec[indices[i-1]]+1;
            }
            start[1] = max(cameras_vec[indices[i-1]] + detection_range_vec[indices[i-1]]+1, start[0]);
            if (start[1] > cameras_vec[indices[i]])
            {
                start[1] = cameras_vec[indices[i]];
            }
            end[2] = cameras_vec[indices[i]] + detection_range_vec[indices[i]];
            if (i < n-1 && end[2] >= cameras_vec[indices[i+1]])
            {
                end[2] = cameras_vec[indices[i+1]]-1;
            }
            // leftcount[0]表示当前摄像头左侧独有覆盖区间覆盖的障碍物数量，leftcount[1]表示当前摄像头与前一摄像头重叠覆盖区间覆盖的障碍物数量，leftcount[2]表示当前摄像头右侧独有覆盖区间覆盖的障碍物数量
            int leftcount[3] = {0};
            for (int j = 0; j < 3; j++)
            {
                for (g = index[j]; g < m; g++)
                {
                    if (obstacles_vec[g] < start[j])
                    {
                        continue;
                    }
                    if (obstacles_vec[g] > end[j])
                    {
                        break;
                    }
                    leftcount[j]++;
                }
                index[j] = g;
            }

            int index = i % 2;
            int prev_index = (i - 1) % 2;
            f[index][0] = max(f[prev_index][0] + leftcount[0], f[prev_index][1] + leftcount[1]);
            f[index][1] = max(f[prev_index][0] + leftcount[2], f[prev_index][1] + leftcount[2]);

            // if (i >= 21)
            // {
            //     cout << "i=" << i << " car: " << cameras_vec[indices[i]] << " " << detection_range_vec[indices[i]] << endl;
            //     cout << "i=" << i << " dis: " << left << " " << left1 << " " << right << endl;
            //     cout << "i=" << i << " leftcount: " << leftcount[0] << " " << leftcount[1] << " " << leftcount[2] << endl;
            //     cout << "i=" << i << " index: " << index[0] << " " << index[1] << " " << index[2] << endl;
            // }
            // cout << "i=" << i << " " << f[i][0] << " " << f[i][1] << endl;
        }

#ifdef log
        // 获取结束时间点
        auto end = std::chrono::high_resolution_clock::now();
        // 计算持续时间（以毫秒为单位）
        auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        auto duration_mid0 = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start).count();
        auto duration_mid = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid).count();
        std::cout << "函数执行时间: " << duration_seconds << " 毫秒" << std::endl;
        std::cout << "阶段1执行时间: " << duration_mid0 << " 毫秒" << std::endl;
        std::cout << "阶段2执行时间: " << duration_mid << " 毫秒" << std::endl;
#endif
        int index1 = (n-1) % 2;
        return max(f[index1][0], f[index1][1]);
    }
};

#define INPUT_FROM_FILE 1


int main(int argc, char* argv[]) {
#ifdef log
    auto start = std::chrono::high_resolution_clock::now();
#endif
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

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

    vector<int> cameras_vec(n);
    vector<int> detection_range_vec(n);
    vector<int> obstacles_vec(m);

    for (int i = 0; i < n; i++) {
        *input_stream >> cameras_vec[i];
    }
    for (int i = 0; i < n; i++) {
        *input_stream >> detection_range_vec[i];
    }
    for (int i = 0; i < m; i++) {
        *input_stream >> obstacles_vec[i];
    }

    Solution solution;

#ifdef log
    // 获取中间时间点
    auto mid = std::chrono::high_resolution_clock::now();
#endif

    int total_obstacles_covered = solution.maxWalls(cameras_vec, detection_range_vec, obstacles_vec);

    cout << total_obstacles_covered << endl;

// 获取结束时间点
#ifdef log
    auto end = std::chrono::high_resolution_clock::now();
    // 计算持续时间（以毫秒为单位）
    auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    auto duration_mid = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid).count();
    std::cout << "函数执行时间: " << duration_seconds << " 毫秒" << std::endl;
    std::cout << "中间函数执行时间: " << duration_mid << " 毫秒" << std::endl;
#endif

    return 0;
}
