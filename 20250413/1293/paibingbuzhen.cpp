  
                                                                                                                  #include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <iostream>
#include <climits>
using namespace std;

using LL = long long;  // 关键类型别名

struct Node {
    LL max_val;
    LL min_val;
    Node() : max_val(LLONG_MIN), min_val(LLONG_MAX) {}
    Node(int val) : max_val(val), min_val(val) {}
    Node operator+(const Node& other) const {
        Node res;
        res.max_val = max(max_val, other.max_val);
        res.min_val = min(min_val, other.min_val);
        return res;
    }
};

class SegmentTree {
private:
    vector<Node> tree;
    int n;

    void build(int node, int l, int r, const vector<LL>& arr) {
        if (l == r) {
            tree[node] = Node(arr[l]);
            return;
        }
        int mid = (l + r) / 2;
        build(2*node, l, mid, arr);
        build(2*node+1, mid+1, r, arr);
        tree[node] = tree[2*node] + tree[2*node+1];
    }

    Node query(int node, int l, int r, int ul, int ur) {
        if (ur < l || ul > r) return Node();
        if (ul <= l && r <= ur) return tree[node];
        int mid = (l + r) / 2;
        return query(2*node, l, mid, ul, ur) + 
               query(2*node+1, mid+1, r, ul, ur);
    }

    void update(int node, int l, int r, int idx, int val) {
        if (l == r) {
            tree[node] = Node(val);
            return;
        }
        int mid = (l + r) / 2;
        if (idx <= mid) update(2*node, l, mid, idx, val);
        else update(2*node+1, mid+1, r, idx, val);
        tree[node] = tree[2*node] + tree[2*node+1];
    }

public:
    SegmentTree(const vector<LL>& arr) {
        n = arr.size();
        tree.resize(4 * n);
        build(1, 0, n-1, arr);
    }

    LL query_diff(int l, int r) {
        Node res = query(1, 0, n-1, l, r);
        return res.max_val - res.min_val;
    }

    void update_val(int idx, int val) {
        update(1, 0, n-1, idx, val);
    }
};

int main() {

    // 输入 m 和 n
    int m, n;
    cin >> m >> n;
    // 输入 m 个数
    vector<LL> nums(m);
    for (int i = 0; i < m; ++i) {
        cin >> nums[i];
    }
    // 输入 n个查询
    vector<pair<int, int>> queries(n);
    for (int i = 0; i < n; ++i) {
        cin >> queries[i].first >> queries[i].second;
    }
    // vector<int> arr = {1, 3, 5, 7, 9};
    SegmentTree st(nums);
    
    // 输出查询结果
    for (const auto& query : queries) {
        int l = query.first - 1; // 转换为0-based索引
        int r = query.second - 1; // 转换为0-based索引
        cout << st.query_diff(l, r) << endl;
    }

    return 0;
}
