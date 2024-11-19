#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>
#include <chrono>

using namespace std;

struct Person {
    int d; // 距离
    int w; // 权重
};

void readfile(int &n, vector<Person> &Persons)
{
    std::ifstream file("yongli/test7337.in");
    // 检查文件是否成功打开
    if (!file.is_open())
    {
        std::cerr << "无法打开文件: shili1000" << std::endl;
        return ; // 返回非零值表示程序异常结束
    }

    // 读取文件内容
    // 文件第一行是n
    file >> n;
    Persons.resize(n);
    // 文件接下来n行是点的坐标
    for (int i = 0; i < n; i++)
    {
        file >> Persons[i].d >> Persons[i].w;
    }

    // 关闭文件
    file.close();
}

int main() {
    int n;

    vector<Person> people;
    readfile(n, people);

    cout << "n: " << n << endl;

    // cin >> n;
    // vector<Person> people(n);
    // for (int i = 0; i < n; ++i) {
    //     cin >> people[i].d >> people[i].w;
    // }

    
    // 获取起始时间点
    auto start = std::chrono::high_resolution_clock::now();

    // 优先队列，存储得分
    // priority_queue<long long int> scores;

    long long int maxArea = 0;
    int s;
    int e;
    for (int i = 0; i < n; ++i) {
        if (i%10000==0)
        {
            cout << "i: " << i << endl;
        }
        for (int j = i + 1; j < n; ++j) {
            int distance = abs(people[i].d - people[j].d);
            int weight = min(people[i].w, people[j].w);
            long long int score = (long long int)distance * weight;
            // scores.push(score);
            if (score > maxArea) {
                s = i;
                e = j;
            }
            maxArea = max(maxArea, score);

        }
    }

    // 取出最大的三个得分
    // long long max_score = scores.top();

    
    // 获取结束时间点
    auto end = std::chrono::high_resolution_clock::now();
    
    // 计算持续时间（以微秒为单位）
    auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "函数执行时间: " << duration_seconds << " 毫秒" << std::endl;

    // scores.pop();
    // long long  second_max_score = scores.top();
    // scores.pop();
    // long long  third_max_score = scores.top();

    cout << "理论最大分数: " << maxArea << endl;
    cout << "第一个人: " << s << " 第二个人: " << e << endl;
    cout << "第一个人的距离: " << people[s].d << " 第一个人的权重: " << people[s].w << endl;
    cout << "第二个人的距离: " << people[e].d << " 第二个人的权重: " << people[e].w << endl;
    cout << "最大面积: " << min(people[s].w,people[e].w) * abs((long long int)people[s].d - people[e].d) << endl;
    // cout << "第二大分数: " << second_max_score << endl;
    // cout << "第三大分数: " << third_max_score << endl;

    return 0;
}