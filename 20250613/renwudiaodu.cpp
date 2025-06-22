#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <map>

#define log 1

#ifdef log
#include <chrono>
#endif

#define input_file 0
#ifdef input_file
#include <fstream>
#endif

using namespace std;

long long int c = 0;

vector<int> flag;

void digui(map<int, int>& dp, int index, vector<vector<int>>& moles, vector<int> &path) {

    // if (c++ % 1000 == 0)
    {
        // cout << "c = " << c << " index = " << index << endl;
    }

    if (index == 0 )
    {
        dp[index] = 0; // 更新 dp
        flag[index] = 0; // 更新 flag
        path[index] = 0;
        return;
    }

    int i = path[index - 1];

    //  printf("[%d] i:%d dp[i]:%d flag[i]:%d path[i]:%d\n", __LINE__, i, dp[i], flag[i], path[i]);
    if (moles[index][0] - moles[i][0] >= abs(moles[index][1] - moles[i][1]) + abs(moles[index][2] - moles[i][2]))
    {
        // printf("[%d] i:%d dp[index]:%d dp[i]:%d\n", __LINE__, i, dp[index], dp[i]);
        dp[index] = dp[i] + 1; // 可以从开始完成第 n 个任务

        // printf("[%d] i:%d dp[index]:%d dp[i]:%d\n", __LINE__, i, dp[index], dp[i]);

        flag[index] = dp[index];
        path[index] = index;
        // printf("[%d] index:%d dp[index]:%d dp[i]:%d flag[index]:%d path[index]:%d\n", __LINE__, index, dp[index], dp[i], flag[index], path[index]);
        // cout << "c = " << c << " index = " << index << " A dp[index] = " << dp[index] << " flag[index]=" << flag[index] << " path[index]=" << path[index] << endl;
        return;
    }
    dp[index] = dp[i];
    int temp = 0;
    flag[index] = 0;
    path[index] = i;
    bool biaoji = false;
    // int j = i > 0 ? i-1 : i;
    int j = index - 1;
    // int j = i;

    int temp_record = 0;

    for (; j >= 0; --j)
    {
        if (moles[j][0] == 0 && j != 0)
        {
            continue;
        }
        if (moles[index][0] - moles[j][0] >= abs(moles[index][1] - moles[j][1]) + abs(moles[index][2] - moles[j][2]))
        {
            if (flag[j] + 1 > temp)
            {
                temp = flag[j] + 1; // 更新最大数量
            }
            if (temp == dp[index])
            {
                biaoji = true; // 找到一个可达的路径
                //  printf("[%d] index:%d i:%d j:%d dp[index]:%d dp[i]:%d flag[j]:%d-----------------\n", __LINE__, index, i, j, dp[index], dp[i], flag[j]);
                //  cout << "c = " << c << " index = " << index << " dp[index] = " << dp[index] << " flag[index]=" << flag[index] << " path[index]=" << path[index] << endl;
                temp_record = j; // 记录路径
                if (dp[j] < dp[index])
                {
                    break;
                }
            }
            if (temp > dp[index])
            {
                if (biaoji)
                {
                    // printf("[%d] index:%d i:%d j:%d dp[index]:%d dp[i]:%d dp[j]:%d flag[j]:%d temp_record:%d dp[record]:%d,flag[record]:%d\n", __LINE__, index, i, j, dp[index], dp[i], dp[j], flag[j], temp_record, dp[temp_record], flag[temp_record]);
                }
                // flag[index] = flag[j] + 1; // 更新 flag
                // path[index] = index; // 记录路径
                biaoji = true; // 找到一个可达的路径

                // cout << "c = " << c << " index = " << index << " dp[index] = " << dp[index] << " flag[index]=" << flag[index] << " path[index]=" << path[index] << endl;

                break;
            }
        }
    }
    flag[index] = temp; // 更新 flag
    if (biaoji && flag[index] >= dp[index])
    {

        dp[index] = flag[index]; // 更新 dp
        path[index] = index;     // 记录路径
    }
    // cout << "c = " << c << " index = " << index << " dp[index] = " << dp[index] << " flag[index]=" << flag[index] << " path[index]=" << path[index] << endl;
    // printf("[%d] index:%d dp[index]:%d dp[i]:%d flag[index]:%d path[index]:%d\n", __LINE__, index, dp[index], dp[i], flag[index], path[index]);
}

class Solution {
public:
    int getMaximumNumber(vector<vector<int>>& moles) {
        // 按照时间戳 t 排序
        sort(moles.begin(), moles.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0] < b[0];
        });


        int n = moles.size();
        if (n == 0) return 0;
        

        // 复制一下moles的数据，从i=1开始
        vector<vector<int>> moles1(n+1);
        moles1[0] = {0, 1, 1}; // 起始位置
        // printf("i=%d %d %d %d\n", 0, 0 ,1, 1);
        for (int i = 0; i < n; ++i)
        {
            moles1[i+1] = moles[i];
            // printf("i=%d %d %d %d\n", i+1, moles1[i+1][0], moles1[i+1][1], moles1[i+1][2]);
        }

        // dp 表示到第t时间，做第n个任务的最大数量
        map<int, int> dp;
        

        vector<int> path(n+1, 0); // 用于记录路径
        flag.resize(n+1, 0); // 用于记录每个任务的可达状态

        for (int i = 0; i < n+1; ++i) {
            // dp[i] = 0; // 初始化 dp
            digui(dp,i, moles1, path);

        }

        return dp[n];
    }
};

int main()
{
    // 输入个数n，接下来输入n行，每行3个整数，表示任务请求时间戳 t 和位置 (x, y)
    // 测试函数耗时
    // 获取起始时间点
#ifdef log
    auto start = std::chrono::high_resolution_clock::now();
#endif

#ifdef input_file
    ifstream input("yongli/test2.in");
    if (!input.is_open()) {
        cout << "无法打开文件" << endl;
        return 1; // 返回非零值表示程序异常结束
    }
    int n;
    input >> n;
    vector<vector<int>> moles(n, vector<int>(3));
    for (int i = 0; i < n; ++i) {
        input >> moles[i][0] >> moles[i][1] >> moles[i][2];
    }
    input.close();
#else
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    int n;
    cin >> n;
    vector<vector<int>> moles(n, vector<int>(3));
    for (int i = 0; i < n; ++i) {
        scanf("%d %d %d", &moles[i][0], &moles[i][1], &moles[i][2]);
    }
#endif
    Solution solution;

#ifdef log
    // 获取中间时间点
    auto mid = std::chrono::high_resolution_clock::now();
#endif

    int result = solution.getMaximumNumber(moles);
    cout << result << endl;
    
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