#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <ctime>
#include <time.h>
#include <chrono>
#include <map>
using namespace std;
int n;
vector<int> value;
map<int,int> low;
map<int,int> high;
int main()
{
    //从文件中读出数据
    ifstream infile("in.out");
    if (!infile) {
        cerr << "无法打开文件 in.out" << endl;
        return 0;
    }

    infile >> n;
    value.resize(n);
    long long int sum = 0;
    for (int i = 0; i < n; i++) {
        infile >> value[i];
        sum += value[i];
    }
    infile.close();
    // 输出读取的值以验证
    cout << "n: " << n << endl;

    // cin >> n;
    // value.resize(n);
    // long long int sum = 0;
    // for (int i = 0; i < n; i++)
    // {
    //     cin >> value[i];
    //     sum += value[i];
    // }

    /**
     * linux平台获取函数执行时间
    */
    // 获取起始时间点
    auto start = std::chrono::high_resolution_clock::now();

    double per = sum * 1.0 / n;
    cout << "per:" << per << endl;
    for (int i = 0; i < n; i++)
    {
        int temp = value[i];
        if (temp <= per) 
        {
            low[temp] += 1;
        }
        else if (temp > per)
        {
            high[temp] += 1;
        }
    }
    
    cout << "low.size:" << low.size() << " high.size:" << high.size() << endl;
    auto med = std::chrono::high_resolution_clock::now();

    // cout << "low:" << endl;
    // for (auto it = low.begin(); it != low.end(); it++)
    // {
    //     cout << it->first << " " << it->second << endl;
    // }
    // cout << "high:" << endl;
    // for (auto it = high.begin(); it != high.end(); it++)
    // {
    //     cout << it->first << " " << it->second << endl;
    // }
    auto it = low.begin();
    if (low.size() == 0 || high.size() == 0)
    {
        cout << 0 << endl;
        return 0;
    }
    // 获取high的最后一个元素

    auto it1 = --high.end();
    long long int res = 0;
    int count = 0;
    int lowmax = 0;
    int highmin = 0;
    while (it != low.end())
    {
        count++;
        // cout << "it->first:" << it->first << " it1->first:" << it1->first << endl;
        if (it->first < it1->first - it1->first / 2)
        {
            int lowvalue = it->first * 2;
            int highvalue = it1->first / 2;

            int lowtemp = it->second;
            int hightemp = it1->second;
            int beishu = min(lowtemp, hightemp);
            // int beishu = 1;
            res += (it1->first - it1->first / 2 - it->first) * beishu;
            it->second -= beishu;
            it1->second -= beishu;
            if (it->second == 0)
            {
                low.erase(it);
            }
            if (it1->second == 0) 
            {
                high.erase(it1);
            }
            if (lowvalue < per)
            {
                low[lowvalue] += beishu;
            }

            if (highvalue > per)
            {
                high[highvalue] += beishu;
            }
            it = low.begin();
            if (high.size() == 0)
            {
                break;
            }
            it1 = --high.end();
        }
        else
        {
            break;
        }
    }
    // 获取结束时间点
    auto end = std::chrono::high_resolution_clock::now();
    
    // 计算持续时间（以微秒为单位）
    auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    auto duration_seconds1 = std::chrono::duration_cast<std::chrono::milliseconds>(med - start).count();
    std::cout << "函数排序时间: " << duration_seconds1 << " 毫秒" << std::endl;
    std::cout << "函数执行时间: " << duration_seconds << " 毫秒" << std::endl;
    
    cout << "count:" << count << endl;

    cout << res << endl;

    return 0;
}