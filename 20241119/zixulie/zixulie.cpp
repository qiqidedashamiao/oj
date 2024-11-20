#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>

using namespace std;

int main()
{
    int n, k;
    cin >> n >> k;
    
    // 获取起始时间点
    auto start = std::chrono::high_resolution_clock::now();

    long long int sum = 0;
    int yu = 1000000007;
    sum = ((1+n) * n / 2) % yu;
    for (int i = 0; i < n; ++i)
    {
        sum += ((n - i - 1) / k + ((n - i - 1) / k + 1) * (i / k)) * (i + 1) % yu;
        sum %= yu;
    }
    
    // 获取结束时间点
    auto end = std::chrono::high_resolution_clock::now();
    
    // 计算持续时间（以微秒为单位）
    auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "函数执行时间: " << duration_seconds << " 毫秒" << std::endl;

    cout << sum << endl;
    return 0;
}