
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <ctime>
#include <time.h>
#include <chrono>

using namespace std;

/*
题意可以等价为，对任意点 A，找出两个经过 A 三元环（三角形）ABC 和 AB ′C ′，使得其经过的点的权值最大。

如果可以O(m)求出每个点作为点A时的答案,就能O(nm)求出原题答案.
为了方便,记当前枚举到的点为A.(代码中对应为i).以下所有的"最优解"指的都是确定点A的最优解.

1.预处理后,O(1)时间判断每条边是否和A构成三元环.
方法: 预处理出adj数组,adj[u]表示点u和A是否相邻.
那么如果(adj[u] and adj[v]),(u,v)就可以和A构成三元环.

2.考虑所有可以和A构成三元环的边(u,v).在只考虑正确性的情况下,只要两两枚举这些边对即可,但总复杂度为O(nm^2)

3.对(u,v)根据value[u]+value[v]进行排序,复杂度为O(mlogm).可以在枚举点A之前排序,复杂度不用再乘上n.

4.结论:至少有一个最优解包含至少一个value[u]+value[v]前3大的边(u,v).

5. 那么现在枚举边对只需要考虑前3条边和其他边的组合,复杂度降为O(n⋅3m).
(事实上,由于在这步中不可能构造出n=m=10000,且每个点都和每条边构成三元环的数据,因此常数会比理论值要小)
*/

constexpr int maxn = 10000;
int adj[maxn], used[maxn];

int maxWeight(vector<vector<int>> &edges, vector<int> &value)
{
    int n = value.size();
    int m = edges.size();
    vector<pair<int, int>> edge;
    for (auto &v : edges)
        edge.push_back({v[0], v[1]});
    vector<vector<int>> G(n);
    // 按照边排序
    sort(edge.begin(), edge.end(), [&](const pair<int, int> &p, const pair<int, int> &q)
         { return value[p.first] + value[p.second] > value[q.first] + value[q.second]; });
    for (auto & item: edge)
    {
        int u, v;
        u = item.first;
        v = item.second;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    // for (int i = 0; i < n; i += 1)
    // {
    //     cout << "i:" << i << endl;
    //     for (int &v : G[i])
    //     {
    //         cout << v << " ";
    //     }
    //     cout << endl;
    // }
    int ans = 0;
    for (int i = 0; i < n; i += 1)
    {
        for (int &v : G[i])
            adj[v] = 1;
        vector<int> ve;
        for (int k = 0; k < m; k += 1)
            if (adj[edge[k].first] && adj[edge[k].second])
                ve.push_back(k);
        
        for (int j = 0; j < min(3, (int)ve.size()); j += 1)
        {
            used[edge[ve[j]].first] = used[edge[ve[j]].second] = 1;
            int bans = value[i] + value[edge[ve[j]].first] + value[edge[ve[j]].second];
            for (int &k : ve)
            {
                int pans = bans;
                if (!used[edge[k].first])
                    pans += value[edge[k].first];
                if (!used[edge[k].second])
                    pans += value[edge[k].second];
                if (pans > ans)
                    ans = pans;
            }
            used[edge[ve[j]].first] = used[edge[ve[j]].second] = 0;
        }
        for (int &v : G[i])
            adj[v] = 0;
    }
    return ans;
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