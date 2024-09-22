#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    // 设置随机数种子
    srand(static_cast<unsigned int>(time(0)));

    // 设置 n 的大小
    int n = 1000000;
    vector<int> value(n);

    // 生成随机数并存储在 value 中
    for (int i = 0; i < n; i++) {
        value[i] = rand() % 1000 + 1; // 生成 0 到 999999 之间的随机数
    }

    // 打开文件进行写入
    ofstream outfile("in.out");
    if (!outfile) {
        cerr << "无法创建文件 in.out" << endl;
        return 1;
    }

    // 写入 n 和 value 的值
    outfile << n << endl;
    for (int i = 0; i < n; i++) {
        outfile << value[i] << endl;
    }

    outfile.close();
    cout << "数据已写入文件 in.out" << endl;

    return 0;
}