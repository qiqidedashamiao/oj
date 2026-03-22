//#pragma GCC optimize("O3,unroll-loops")
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <cstring>  
//#define int long long
using namespace std;
const int MAXN = 200001;
const int MAXM = 25;
const int INF = 1000000001;
int n, k;
typedef pair<int, int> pii;
vector<int> grid[MAXN];
bool vis[MAXN];
int siz[MAXN];
int curCnt[MAXN];
long long curSum[MAXN];
int allCnt[MAXN];
long long allSum[MAXN];
long long ans = 0;
int Group[MAXN];
void getSize(int u, int f) {
    siz[u] = 1;
    for (int v : grid[u]) {
        //int v = edge.first;
        if (v == f || vis[v]) continue;
        getSize(v, u);
        siz[u] += siz[v];
    }
}
int getCentroid(int u, int f) {
    getSize(u, f);
    int half = siz[u] >> 1;
    bool find = false;
    while (!find) {
        find = true;
        for (int v : grid[u]) {
            //int v = edge.first;
            if (v == f || vis[v] || siz[v] < half) continue;
            f = u;
            u = v;
            find = false;
            break;
        }
    }
    return u;
}
void dfs(int u, int f, int d) {
    int g = Group[u];
    curCnt[g]++;
    curSum[g] += d;
    for (int v : grid[u]) {
        //int v = edge.first;
        //int w = edge.second;
        if (v == f || vis[v]) continue;
        dfs(v, u, d + 1);
    }
}
void calc(int u) {
    for (int i = 0; i <= 20; i++) allCnt[i] = allSum[i] = 0;
    allCnt[Group[u]] = 1;
    for (int v : grid[u]) {
        //int v = edge.first;
        //int w = edge.second;
        if (vis[v]) continue;
        for (int i = 0; i <= 20; i++) curCnt[i] = curSum[i] = 0;
        dfs(v, u, 1);
        for (int i = 0; i <= 20; i++) ans += (long long)curCnt[i] * allSum[i] + (long long)allCnt[i] * curSum[i];
        for (int i = 0; i <= 20; i++) allCnt[i] += curCnt[i], allSum[i] += curSum[i];
    }
}
void solve(int u) {
    vis[u] = true;
    calc(u);
    for (int v : grid[u]) {
        if (vis[v]) continue;
        solve(getCentroid(v, u));
    }
}
class Solution {
public:
    long long interactionCosts(int n, vector<vector<int>>& edges, vector<int>& group) {
        for(int i = 1; i <= n; i++) grid[i].clear(), vis[i] = false;
        for(auto edge : edges){
            int u = edge[0];
            int v = edge[1];
            u++, v++;
            grid[u].push_back(v);
            grid[v].push_back(u);
        }
        for(int i = 1; i <= n; i++) Group[i] = group[i - 1];
        ans = 0;
        solve(1);
        return ans;
    }
};


#define log 1
#ifdef log
#include <chrono>
#endif
#define INPUT_FROM_FILE 1
#include <fstream>

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

    int n;
    *input_stream >> n;

    vector<vector<int>> edges(n-1, vector<int>(2));

    vector<int> group(n);

    for (int i = 0; i < n-1; i++)
    {
        *input_stream >> edges[i][0] >> edges[i][1];
    }
    for (int i = 0; i < n; i++) {
        *input_stream >> group[i];
    }
#ifdef log
    // 获取中间时间点
    auto mid = std::chrono::high_resolution_clock::now();
#endif
    Solution solution;
    long long sum = solution.interactionCosts(n, edges, group);
#ifdef log
    // 获取结束时间点
    auto end = std::chrono::high_resolution_clock::now();
    // 计算持续时间（以毫秒为单位）
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid).count();
    auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "函数执行时间: " << time << " 毫秒" << std::endl;
    std::cout << "总执行时间: " << duration_seconds << " 毫秒" << std::endl;
#endif
    cout << sum << endl;
    return 0;
}
