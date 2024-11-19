#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>

using namespace std;

struct Point {
    int x, y;
};

bool compare(const Point &a, const Point &b) {
    return a.x < b.x;
}

int s, e, minh;

long long int maxRectangleArea(vector<Point> &points) {
    int n = points.size();
    //sort(points.begin(), points.end(), compare);

    int left = 0, right = n - 1;
    long long int maxArea = 0;

    while (left < right) {
        int height = min(points[left].y, points[right].y);
        int width = points[right].x - points[left].x;
        long long int area = (long long int)height * width;
        if (area > maxArea) {
            s = left;
            e = right;
            minh = height;
        }
        maxArea = max(maxArea, area);

        if (points[left].y < points[right].y) {
            left++;
        } else {
            right--;
        }
    }

    return maxArea;
}

void readfile(int &n, vector<Point> &points)
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
    points.resize(n);
    // 文件接下来n行是点的坐标
    for (int i = 0; i < n; i++)
    {
        //Point p;
        file >> points[i].x >> points[i].y;
        //points.push_back(p);
    }

    // 关闭文件
    file.close();
}



int main() {

    // long long int  a = 697257;
    // long long int  b = 687342;
    // long long int  c = a-b;
    // long long int  d = 1516152;
    // long long int  e = c*d;
    // cout << c << endl;
    // cout << e << endl;  

    int n;

    vector<Point> points;
    readfile(n, points);

    cout << "n: " << n << endl;
    
    //cin >> n;
    //vector<Point> points(n);
    // for (int i = 0; i < n; i++) {
    //     cin >> points[i].x >> points[i].y;
    // }

    // 获取起始时间点
    auto start = std::chrono::high_resolution_clock::now();

    long long int result = maxRectangleArea(points);

    // 获取结束时间点
    auto end = std::chrono::high_resolution_clock::now();
    
    // 计算持续时间（以微秒为单位）
    auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "函数执行时间: " << duration_seconds << " 毫秒" << std::endl;

    cout << result << endl;
    cout << s << " " << e << endl;
    cout << points[s].x << " " << points[s].y << endl;
    cout << points[e].x << " " << points[e].y << endl;
    long long int area = min(points[s].y, points[e].y) * ((long long int)points[e].x - points[s].x);
    cout << area << endl;

    return 0;
}