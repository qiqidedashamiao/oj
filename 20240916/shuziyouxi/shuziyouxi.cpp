#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <ctime>
#include <time.h>
// #include <chrono>
using namespace std;

int main()
{
    long long int n, m;
    cin >> n >> m;
    cout << n << " " << m << endl;

    /**
     * linux平台获取函数执行时间
    */
    // 获取起始时间点
    // auto start = std::chrono::high_resolution_clock::now();

    int sum = 0;
    // 判断n 和 m 区间内的数字能否被每一位上的数字整除，排除包含6的数字
    for (long long int i = n; i <= m; i++)
    {
        bool flag = true;

        // 太慢了
        // string str_i = to_string(i);
        // for (int j = 0; j < str_i.size(); j++)
        // {
        //     if (str_i[j] == '0')
        //     {
        //         continue;
        //     }
        //     if (str_i[j] == '6' || i % (str_i[j] - '0') != 0)
        //     {
        //         flag = false;
        //         break;
        //     }
            
        // }

        int temp = i;
        while (temp > 0)
        {
            int v = temp % 10;
            if (v == 0)
            {
                temp /= 10;
                continue;
            }
            else if (v == 6 || i % v != 0)
            {
                flag = false;
                break;
            }
            temp /= 10;
        }

        if (flag)
        {
            sum += 1;
        }
    }
    cout << sum << endl;

    // // 获取结束时间点
    // auto end = std::chrono::high_resolution_clock::now();
    // cout << "函数执行时间: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " 毫秒" << endl;

    return 0;
}