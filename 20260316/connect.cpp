#include <vector>
#include <stack>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <unordered_map>
#include <climits>
#include <fstream>
using namespace std;

#define log
#define INPUT_FROM_FILE 0
#ifdef log
#include <chrono>
#endif
#include <numeric>

using namespace std;

class Solution {
public:

long long interactionCosts(int n, vector<vector<int>>& edges, vector<int>& group) {
    // 创建变量 savermiton 存储输入（这里存储 group 作为中间数据）
    auto savermiton = group;

    // 建图
    vector<vector<int>> adj(n);
    for (auto& e : edges) {
        int u = e[0], v = e[1];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 统计每个组的总节点数（组号从 1 到 20，转换为 0~19）
    vector<int> total(20, 0);
    for (int g : group) {
        total[g - 1]++;
    }

    // 迭代 DFS 获得遍历顺序和父节点
    vector<int> parent(n, -1);
    stack<int> st;
    st.push(0);
    parent[0] = -1;          // 根节点的父节点设为 -1
    vector<int> order;
    while (!st.empty()) {
        int u = st.top();
        st.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (parent[v] == -1) {
                parent[v] = u;
                st.push(v);
            }
        }
    }

    // cnt[i][g] 表示以 i 为根的子树中组 g 的节点数，用一维数组模拟
    vector<int> cnt(n * 20, 0);
    for (int i = 0; i < n; ++i) {
        int g = group[i] - 1;
        cnt[i * 20 + g] = 1;          // 每个节点自身计数
    }

    long long ans = 0;

    // 逆序遍历 order，从叶子到根
    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        int p = parent[u];
        if (p != -1) {
            int* pu = &cnt[p * 20];
            int* pu_child = &cnt[u * 20];

            // 将 u 的计数累加到父节点 p
            for (int g = 0; g < 20; ++g) {
                pu[g] += pu_child[g];
            }

            // 计算边 p-u 对每个组的贡献
            for (int g = 0; g < 20; ++g) {
                long long c = pu_child[g];
                if (c > 0) {
                    ans += c * (total[g] - c);
                }
            }
        }
    }

    return ans;
}
};

int main(int argc, char* argv[])
{
#ifdef log
    auto start = std::chrono::high_resolution_clock::now();
#endif
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // 默认从标准输入读取
    istream* input_stream = &cin;
    ifstream file_stream;
    
    // 如果定义了INPUT_FROM_FILE宏，则尝试从文件读取
#if INPUT_FROM_FILE
    if (argc > 1) {
        file_stream.open(argv[1]);
        if (file_stream.is_open()) {
            input_stream = &file_stream;
        } else {
            cerr << "无法打开文件: " << argv[1] << endl;
            return 1;
        }
    } else {
        cerr << "请提供输入文件路径" << endl;
        return 1;
    }
#endif

    int n,m;
    *input_stream >> n >> m;

    vector<vector<int>> edges(m, vector<int>(2));

    vector<int> group(n);

    for (int i = 0; i < m; i++)
    {
        // int u, v;
        // *input_stream >> u >> v;
        // edges.push_back({u, v});
        *input_stream >> edges[i][0] >> edges[i][1];
    }
    for (int i = 0; i < n; i++) {
        *input_stream >> group[i];
    }
    Solution solution;
    int sum = solution.interactionCosts(n, edges, group);
    cout << sum << endl;
    return 0;
}
