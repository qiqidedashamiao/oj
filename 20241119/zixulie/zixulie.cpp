#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
// #include <chrono>

using namespace std;

int main()
{
    int n, k;
    cin >> n >> k;
    vector<int> nums(n);

    for(int i = 0; i < n; ++i)
    {
        cin >> nums[i];
    }
    
    // 获取起始时间点
    // auto start = std::chrono::high_resolution_clock::now();

    long long int sum = 0;
    int yu = 1000000007;
    sum = ((1+n) * n / 2) % yu;
    int i = 0;
    int diff = 0;
    int index = 0;
    vector<int> f(n,0);
    while (i < n)
    {
        if (abs(nums[i] - nums[i-1]) == k && nums[i] - nums[i-1] == diff)
        {

            f[i - index] = f[i - index-1] * 2 - f[i - index - 2] + nums[i] * (i - index) + nums[i-1];
            if (i-index == 2)
            {
                f[i - index] += f[i - index - 2];
            }
            // cout << i << "=" << i << " " << f[i-index] << endl;
            if (i == n-1)
            {
                sum += f[i - index];
            }
        }
        else if (abs(nums[i] - nums[i-1]) == k)
        {
            index = i - 1;
            diff = nums[i] - nums[i - 1];
            f[i - index] = nums[i] + f[i - index - 1];
            // cout << i << "=" << i << " " << f[i-index] << endl;
            if (i == n-1)
            {
                sum += f[i - index];
            }
        }
        else
        {
            diff = 0;
            //sum += f[i - index - 1];
            index = i;
            f[i - index] = nums[i];
            // cout << i << "=" << i << " " << f[i-index] << endl;
        }
        ++i;
    }

    /* for (int i = 0; i < n; ++i)
    {
        sum += ((n - i - 1) / k + ((n - i - 1) / k + 1) * (i / k)) * (i + 1) % yu;
        sum %= yu;
    } */
    
    // // 获取结束时间点
    // auto end = std::chrono::high_resolution_clock::now();
    
    // // 计算持续时间（以微秒为单位）
    // auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    // std::cout << "函数执行时间: " << duration_seconds << " 毫秒" << std::endl;                                                                                                                                                           

    cout << sum << endl;
    return 0;
}