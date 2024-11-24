#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
// #include <chrono>
#include <fstream>

using namespace std;

const int MOD = 1000000007;

void readfile(int &n, int &k, vector<int> &nums)
{
    std::ifstream file("test3.in");
    // 检查文件是否成功打开
    if (!file.is_open())
    {
        std::cerr << "无法打开文件: shili1000" << std::endl;
        return ; // 返回非零值表示程序异常结束
    }

    // 读取文件内容
    // 文件第一行是n
    file >> n >> k;
    nums.resize(n);
    // 文件接下来n行是点的坐标
    for (int i = 0; i < n; i++)
    {
        //Point p;
        file >> nums[i];
        //points.push_back(p);
    }

    // 关闭文件
    file.close();
}

int main() {
    int n, k;

    //从文件test3.in中读取入参数据
    // vector<int> nums;
    // readfile(n, k, nums);

    cin >> n >> k;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    // cout << "nums.size()=" << nums.size() << endl;
    //  // 获取起始时间点
    // auto start = std::chrono::high_resolution_clock::now();

    map<int, long long int> f;
    map<int, int> count;
    long long int result = 0;

    for (int i = 0; i < n; ++i)
    {
        int item = nums[i];
        if (f.find(item) == f.end())
        {
            f[item] = 0;
            count[item] = 0;
        }
        f[item] = (f[item] + item) % MOD;
        count[item] = count[item] + 1;


        if (f.find(item - k) != f.end())
        {
            f[item] = (f[item] + f[item - k]  + (item * count[item-k])%MOD) % MOD;
            count[item] = count[item] + count[item-k];
        }
        if (f.find(item + k) != f.end())
        {
            f[item] = (f[item] + f[item + k]  + (item * count[item+k])%MOD) % MOD;
            count[item] = count[item] + count[item+k];
        }
        // long long int &a = f[item - k];
        // long long int &b = f[item + k];
        // int &c = count[item - k];
        // int &d = count[item + k];
        // long long int &e = f[item];
        // int &g = count[item];
        // f[item] = ((e + a + b)%MOD + (item * c)%MOD + (item * d)%MOD + item)%MOD;
        // count[item] = g + c + d + 1;
        // cout << "i=" << i << " " << "item=" << item << " f[item]=" << f[item] << " count[item]="  << count[item] << endl;
        
    }
    // cout << "f.size()=" << f.size() << endl;
    for (auto &item : f)
    {
        result = (result + item.second) % MOD;
    }

    // // 获取结束时间点
    // auto end = std::chrono::high_resolution_clock::now();
    
    // // 计算持续时间（以微秒为单位）
    // auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    // std::cout << "函数执行时间: " << duration_seconds << " 毫秒" << std::endl;

    cout << result << endl;
    return 0;
}