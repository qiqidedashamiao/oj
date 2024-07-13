#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <chrono>
using namespace std;


/**
 * 
*/
int total = 0;
int total1 = 0;
int total2 = 0;
int totalmin__ = INT_MAX;
vector<int> dp__;
vector<int> dp_min__;
void minArray(vector<int> &nums, int index, int min__, int max__, vector<int> dp, vector<int> &dp_min)
{
    total++;
    // if (total % 1000000 == 0)
    // {
    //     cout << "total=" << total << endl;
    // }
    if (index == nums.size())
    {
        // cout << "total   =" << total << endl;
        // cout << "total1  =" << total1 << endl;
        // cout << "nums=";
        // for (int i = 0; i < nums.size(); i++)
        // {
        //     cout << nums[i] << " ";
        // }
        // cout << endl;
        
        // cout << "max__=" << max__ << " min__=" << min__ << endl;
        int res = max(abs(max__), abs(min__));
        if (totalmin__ > res)
        {
            dp__ = dp;
            dp_min__ = dp_min;
        }
        totalmin__ = min(totalmin__, res);
        return;
    }
    if (index > 0 && max(abs(max__), abs(min__)) >= totalmin__)
    {
        total1++;
        // if(total1 % 10000 == 0)
        // {
        //     cout << "total1=        " << total1 << endl;
        // }
        return;
    }
    // if (index % 2 == 0)
    // {
    //     nums[index] = -nums[index];
    // }
    int tempmax__ = max__;
    int tempmin__ = min__;

    dp[index] = max(dp[index - 1] + nums[index], nums[index]);
    max__ = max(tempmax__, dp[index]);
    dp_min[index] = min(dp_min[index - 1] + nums[index], nums[index]);
    min__ = min(tempmin__, dp_min[index]);

    // if (index > 0 && max(abs(max__), abs(min__)) >= totalmin__)
    // {
    //     total1++;
    //     if(total1 % 1000000 == 0)
    //     {
    //         cout << "total1=        " << total1 << "index=" << index << endl;
    //     }
    //     //return;
    // }
    // else
    {
        minArray(nums, index + 1, min__, max__, dp, dp_min);
    }
    
    nums[index] = -nums[index];

    dp[index] = max(dp[index - 1] + nums[index], nums[index]);
    max__ = max(tempmax__, dp[index]);
    dp_min[index] = min(dp_min[index - 1] + nums[index], nums[index]);
    min__ = min(tempmin__, dp_min[index]);

    // if (index > 0 && max(abs(max__), abs(min__)) >= totalmin__)
    // {
    //     total1++;
    //     if(total1 % 1000000 == 0)
    //     {
    //         cout << "total11=        " << total1 << "index=" << index << endl;
    //     }
    //     //return;
    // }
    // else
    {
        minArray(nums, index + 1, min__, max__, dp, dp_min);
    }
    
    nums[index] = -nums[index];
    // if (index % 2 == 0)
    // {
    //     nums[index] = -nums[index];
    // }
}

int unSuitability(vector<int>& operate) 
{
    vector<int> dp(operate);
    vector<int> dp_min(operate.size());
    dp__.resize(operate.size());
    dp_min__.resize(operate.size());
    int max__ = INT_MIN;
    int min__ = INT_MAX;
    operate[0] = -operate[0];
    dp[0] = operate[0];
    dp_min[0] = operate[0];
    max__ = dp[0];
    min__ = dp_min[0];
    minArray(operate, 1, min__, max__, dp, dp_min);
    return totalmin__;
}

int main()
{
    /**
     * 1. 读入数据 输入大小n，然后输入n个数存入数组
    */
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; i++)
    {
        cin >> nums[i];
    }
    /**
     * linux平台获取函数执行时间
    */
    // 获取起始时间点
    auto start = std::chrono::high_resolution_clock::now();
    
    unSuitability(nums);
    // 获取结束时间点
    auto end = std::chrono::high_resolution_clock::now();
    
    // 计算持续时间（以微秒为单位）
    // auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    // std::cout << "函数执行时间: " << duration_seconds << " 毫秒" << std::endl;
    // cout << "total=" << total << endl;
    // cout << "total1=" << total1 << endl;
    cout << totalmin__ << endl;

    return 0;
}