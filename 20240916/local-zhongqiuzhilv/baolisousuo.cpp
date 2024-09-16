
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <ctime>
#include <time.h>
#include <chrono>

using namespace std;

constexpr int maxn = 10000;
int adj[maxn], used[maxn];

int maxWeight(vector<vector<int>> &edges, vector<int> &value)
{
    int n = value.size();
    int m = edges.size();
    vector<vector<bool> > adjacent(n, vector<bool>(n, false));
    
    for (int i = 0; i < edges.size(); ++i)
    {
        adjacent[edges[i][0]][edges[i][1]] = adjacent[edges[i][1]][edges[i][0]] = true;
    }
    int maxHappiness = 0;

    for (int t = 0; t < n; ++t) { // 遍历每个重点景点t
        for (int k = 0; k < n; ++k) {
            if (adjacent[t][k]) {
                for (int l = 0; l < n; ++l) {
                    if (adjacent[k][l] && adjacent[l][t] && k != l) {
                        // 计算上午和下午游玩的幸福度总和
                        int totalHappiness = value[t] + value[k] + value[l];
                        // 如果k'和l'与k和l不同，则需要添加额外的幸福度
                        bool differentPath = false;
                        for (int kPrime = 0; kPrime < n && !differentPath; ++kPrime) {
                            if (adjacent[t][kPrime] && kPrime != k) {
                                for (int lPrime = 0; lPrime < n; ++lPrime) {
                                    if (adjacent[kPrime][lPrime] && adjacent[lPrime][t] && kPrime != lPrime &&
                                        (kPrime != l || lPrime != k)) {
                                        totalHappiness += value[kPrime] + value[lPrime];
                                        differentPath = true;
                                        break;
                                    }
                                }
                            }
                        }
                        // 如果找不到不同的路径，则只计算上午的路径
                        if (!differentPath) {
                            totalHappiness = (value[t] + value[k] + value[l]) / 2 * 3;
                        }
                        // 更新最大幸福度值
                        maxHappiness = max(maxHappiness, totalHappiness);
                    }
                }
            }
        }
    }
    return maxHappiness;
}

void readfile(vector<int> &value, vector<vector<int> > &edges, int &n, int &m)
{
    std::ifstream file("yongli/in1");
    // 检查文件是否成功打开
    if (!file.is_open())
    {
        std::cerr << "无法打开文件: in" << std::endl;
        return ; // 返回非零值表示程序异常结束
    }
    // 读取文件内容
    // 第一行为 n m
    file >> n >> m;
    value.resize(n);
    edges.resize(m, vector<int>(2));
    for (int i = 0; i < n; i++)
    {
        file >> value[i];
    }
    for (int i = 0; i < m; i++)
    {
        file >> edges[i][0] >> edges[i][1];
    }
    // cout << "edge:" << edges.size() << endl;
    // cout << "value:" << value.size() << endl;
}

int main() {
    int n, m;
    // cin >> n >> m;
    // vector<int> value(n);
    // vector<vector<int> > edges(m, vector<int>(2));
    // for (int i = 0; i < n; i++)
    // {
    //     cin >> value[i];
    // }
    // for (int i = 0; i < m; i++)
    // {
    //     cin >> edges[i][0] >> edges[i][1];
    // }

    // 从文件读取数据
    vector<int> value;
    vector<vector<int>> edges;
    readfile(value, edges, n, m);

    /**
     * linux平台获取函数执行时间
    */
    // 获取起始时间点
    auto start = std::chrono::high_resolution_clock::now();

    int maxHappiness = -1;
    maxHappiness = maxWeight(edges, value);
    
    // 获取结束时间点
    auto end = std::chrono::high_resolution_clock::now();
    
    // 计算持续时间（以微秒为单位）
    auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "函数执行时间: " << duration_seconds << " 毫秒" << std::endl;

    if (maxHappiness == 0) {
        cout << -1 << endl; // 没有找到有效路径
    } else {
        cout << maxHappiness << endl; // 输出最大游玩体验值和
    }

    return 0;
}