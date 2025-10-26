#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <string>
#include<climits>

#define log

#ifdef log
#include <chrono>
#endif

#define input_file
#ifdef input_file
#include <fstream>
#endif

using namespace std;

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
    
    // 重载减法运算符，方便计算向量
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
    
    // 判断两点是否相同（考虑浮点误差）
    bool operator==(const Point& other) const {
        return fabs(x - other.x) < 1e-9 && fabs(y - other.y) < 1e-9;
    }
};



// 计算向量的点积
double dotProduct(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y;
}

// 计算两点之间的距离
double distance(const Point& a, const Point& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// 计算向量的叉积
double crossProduct(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}




// 判断两条线段是否平行（考虑浮点误差）
bool areParallel(const Point& a1, const Point& a2, const Point& b1, const Point& b2) {
    Point vec1 = a2 - a1;
    Point vec2 = b2 - b1;
    
    // 使用叉积判断是否平行
    double cross = crossProduct(vec1, vec2);
    return fabs(cross) < 1e-9;
}

// 判断两条线段是否平行且不共线
bool areSegmentsParallelAndNotCollinear(double x1, double y1, double x2, double y2, 
                                       double x3, double y3, double x4, double y4) {
    // 计算方向向量
    double dx1 = x2 - x1, dy1 = y2 - y1;
    double dx2 = x4 - x3, dy2 = y4 - y3;
    
    // 判断是否平行：方向向量叉积为0
    double cross = dx1 * dy2 - dy1 * dx2;
    if (fabs(cross) > 1e-9) {
        return false; // 不平行
    }
    
    // 如果平行，检查是否在同一条直线上（共线）
    // 方法：检查点(x3,y3)是否在线段A所在的直线上
    // 使用叉积判断三点共线：(x2-x1)*(y3-y1) - (y2-y1)*(x3-x1) == 0
    
    // double collinearCheck = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
    // if (fabs(collinearCheck) < 1e-9) {
    //     return false; // 共线，不是我们要的平行线段
    // }
    
    return true; // 平行且不共线
}

// 计算points中所有的平行边，并且去掉同一条直线上的平行边
// int findParallelEdges(const vector<Point>& points, vector<pair<pair<Point, Point>, pair<Point, Point>>>& parallelEdges) {
//     int n = points.size();
//     int count = 0;
//     map<pair<int,int>, pair<int,int>> usedLines;
//     for (int i = 0; i < n; i++)
//     {
//         if (i+3 >= n)
//         {
//             break;
//         }
//         for (int j = i + 1; j < n; j++) 
//         {
//             // 判断 points[i]-points[j] 这条线段是否已经处理过
//             for (int k = j+1; k < n; k++) 
//             {
//                 for (int l = k + 1; l < n; l++) 
//                 {
//                         // if (areParallel(points[i], points[j], points[k], points[l])) {
//                         //     parallelEdges.push_back({{points[i], points[j]}, {points[k], points[l]}});
//                         // }
//                         if (areSegmentsParallelAndNotCollinear(points[i].x, points[i].y, points[j].x, points[j].y,
//                                                                points[k].x, points[k].y, points[l].x, points[l].y)) 
//                         {
//                             #ifdef log
//                             // cout << "i:" << i << " j:" << j << " k:" << k << " l:" << l << endl;
//                             #endif
//                             count++;

//                             parallelEdges.push_back({{points[i], points[j]}, {points[k], points[l]}});
//                         }
//                         else if (areSegmentsParallelAndNotCollinear(points[i].x, points[i].y, points[k].x, points[k].y,
//                                                                points[j].x, points[j].y, points[l].x, points[l].y)) 
//                         {
//                             // cout << "i:" << i << " k:" << k << " j:" << j << " l:" << l << endl;
//                             count++;
//                             parallelEdges.push_back({{points[i], points[j]}, {points[l], points[k]}});
//                         }
//                         else if (areSegmentsParallelAndNotCollinear(points[i].x, points[i].y, points[l].x, points[l].y,
//                                                                points[j].x, points[j].y, points[k].x, points[k].y)) 
//                         {
//                             // cout << "i:" << i << " l:" << l << " j:" << j << " k:" << k << endl;
//                             count++;
//                             parallelEdges.push_back({{points[i], points[j]}, {points[k], points[l]}});
//                         }
//                 }
//             }
//         }
//     }
//     return count;
// }


int findParallelEdges(const vector<Point>& points, vector<pair<pair<Point, Point>, pair<Point, Point>>>& parallelEdges) {
#ifdef log
    auto start = std::chrono::high_resolution_clock::now();
#endif
    int n = points.size();
    int count = 0;
    map<pair<int,int>, pair<int,int>> usedLines_flag;
    map<pair<int, int>, vector<pair<int, int>>> usedLines;
    vector<vector<int>> processed_point;
    int c = 0;
    int c1 = 0;
    for (int i = 0; i < n; i++)
    {
        if (i+3 >= n)
        {
            break;
        }
        for (int j = i + 1; j < n; j++) 
        {
            if (usedLines_flag.find({i,j}) != usedLines_flag.end())
            {
                // cout << "skip i:" << i << " j:" << j << endl;
                c++;
                continue;
            }
            // 判断 points[i]-points[j] 这条线段是否已经处理过
            for (int k = i+1; k < n; k++) 
            {
                if (j == k)
                {
                    continue;
                }
                
                for (int l = k + 1; l < n; l++) 
                {
                    if (j == l)
                    {
                        continue;
                    }
                    c1++;
                        // if (areParallel(points[i], points[j], points[k], points[l])) {
                        //     parallelEdges.push_back({{points[i], points[j]}, {points[k], points[l]}});
                        // }
                        if (areSegmentsParallelAndNotCollinear(points[i].x, points[i].y, points[j].x, points[j].y,
                                                              points[k].x, points[k].y, points[l].x, points[l].y)) 
                        {
                            #ifdef log
                            // cout << "i:" << i << " j:" << j << " k:" << k << " l:" << l << endl;
                            #endif
                            count++;
                            // usedLines_flag[{k,l}] = {i,j};
                            // if (usedLines.find({i,j}) == usedLines.end())
                            // {
                            //     usedLines[{i,j}] = {{k,l}};
                            // }
                            // else
                            // {
                            //     usedLines[{i,j}].push_back({k,l});
                            // }

                            // parallelEdges.push_back({{points[i], points[j]}, {points[k], points[l]}});
                            // processed_point.push_back({i, j, k, l});
                        }
                        // else if (areSegmentsParallelAndNotCollinear(points[i].x, points[i].y, points[k].x, points[k].y,
                        //                                        points[j].x, points[j].y, points[l].x, points[l].y)) 
                        // {
                        //     // cout << "i:" << i << " k:" << k << " j:" << j << " l:" << l << endl;
                        //     count++;
                        //     parallelEdges.push_back({{points[i], points[j]}, {points[l], points[k]}});
                        // }
                        // else if (areSegmentsParallelAndNotCollinear(points[i].x, points[i].y, points[l].x, points[l].y,
                        //                                        points[j].x, points[j].y, points[k].x, points[k].y)) 
                        // {
                        //     // cout << "i:" << i << " l:" << l << " j:" << j << " k:" << k << endl;
                        //     count++;
                        //     parallelEdges.push_back({{points[i], points[j]}, {points[k], points[l]}});
                        // }
                }
            }
        }
    }

#ifdef log
    // 获取中间时间点
    auto mid = std::chrono::high_resolution_clock::now();
#endif

    // 判断processed_point里面重复的点集合的个数
    map<string, int> used_points;
    int used_points_count = 0;

    for (int i = 0; i < processed_point.size(); i++)
    {
        // 对processed_point二维里面的点进行从小到大排序
        sort(processed_point[i].begin(), processed_point[i].end());
        // 将processed_point[i]里面的点拼接成一个字符串，用_分割
        string key = to_string(processed_point[i][0]) + "_" + to_string(processed_point[i][1]) + "_" + to_string(processed_point[i][2]) + "_" + to_string(processed_point[i][3]);
        if (used_points.find(key) == used_points.end())
        {
            used_points[key] = 1;
        }
        else
        {
            used_points_count++ ;
        }
    }

#ifdef log
    cout << "c1: " << c1 << endl;
    auto end = std::chrono::high_resolution_clock::now();
    // 计算持续时间（以毫秒为单位）
    auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start).count();
    auto duration_mid = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid).count();
    std::cout << "函数第一阶段: " << duration_seconds << " 毫秒" << std::endl;
    std::cout << "函数第二阶段: " << duration_mid << " 毫秒" << std::endl;
#endif
    return count - used_points_count;
}





// 检查四个点是否构成凸四边形
bool isConvexQuadrilateral(const vector<Point>& points) {
    if (points.size() != 4) return false;
    
    // 计算所有相邻边的叉积
    vector<double> crossProducts;
    for (int i = 0; i < 4; i++) {
        Point v1 = points[(i + 1) % 4] - points[i];
        Point v2 = points[(i + 2) % 4] - points[(i + 1) % 4];
        crossProducts.push_back(crossProduct(v1, v2));
    }
    
    // 检查所有叉积是否同号（均为正或均为负）
    bool allPositive = true, allNegative = true;
    bool hasZero = false;
    for (double cp : crossProducts) {
        if (cp < -1e-9) {
            allPositive = false;
        } else if (cp > 1e-9) {
            allNegative = false;
        } else {
            hasZero = true; // 检测到叉积接近0，可能有共线点
        }
    }
    
    // 如果有叉积接近0，说明有点共线，不是严格凸四边形
    if (hasZero) {
        return false;
    }
    
    return allPositive || allNegative;
}

// 计算点的极角（用于排序）
double polarAngle(const Point& center, const Point& p) {
    return atan2(p.y - center.y, p.x - center.x);
}

// 对点进行排序（逆时针方向）
vector<Point> sortPoints(const vector<Point>& points) {
    if (points.size() != 4) return points;
    
    // 计算中心点
    Point center(0, 0);
    for (const Point& p : points) {
        center.x += p.x;
        center.y += p.y;
    }
    center.x /= 4;
    center.y /= 4;
    
    // 复制点并排序
    vector<Point> sorted = points;
    sort(sorted.begin(), sorted.end(), [&](const Point& a, const Point& b) {
        return polarAngle(center, a) < polarAngle(center, b);
    });
    
    return sorted;
}

// 判断是否为梯形
bool isTrapezoid(const vector<Point>& points) {
    vector<Point> sorted = sortPoints(points);
    
    if (!isConvexQuadrilateral(sorted)) {
        return false;
    }
    
    // 检查是否有一组对边平行，另一组不平行
    bool parallel1 = areParallel(sorted[0], sorted[1], sorted[2], sorted[3]);
    if (parallel1) {
        return true;
    }
    bool parallel2 = areParallel(sorted[1], sorted[2], sorted[3], sorted[0]);
    
    return parallel2;
    // 梯形：恰好有一组对边平行
    // return (parallel1 && !parallel2) || (!parallel1 && parallel2);
}



// 计算任意一条线段的斜率
int getSlope(const Point& a, const Point& b) {
    if (fabs(b.x - a.x) < 1e-9) {
        return INT_MAX; // 垂直线段，斜率无穷大
    }
    return (b.y - a.y) / (b.x - a.x);
}

double slope[500][500];

struct info {
    int i;
    int j;
    double slope;
};

int countBySlope(const vector<Point>& points) {
    int count = 0;
    int n = points.size();
    vector<info> slopes((n*n-1)/2);
    int index = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++) {

            // 检查两对平行边是否可以组成梯形
            // slope[i][j] = getSlope(points[i], points[j]);
            slopes[index].i = i;
            slopes[index].j = j;
            slopes[index].slope = getSlope(points[i], points[j]);

        }
    }
    // 按斜率排序
    sort(slopes.begin(), slopes.end(), [](const info& a, const info& b) {
        return a.slope < b.slope;
    });
    return count;
}



class Solution {
public:
    int countTrapezoids(vector<vector<int>>& points)
    {
        int n = points.size();
        vector<Point> points1;
        for (int i = 0; i < n; i++) 
        {
            points1.push_back(Point(points[i][0], points[i][1]));
        }
        int count = 0;
        vector<pair<pair<Point, Point>, pair<Point, Point>>> parallelEdges;
        count = countBySlope(points1);

    return count;
    }
};

// 主函数
int main() {
#ifdef log
    auto start = std::chrono::high_resolution_clock::now();
#endif
#ifdef input_file
    ifstream input("yongli/test3.in");
    if (!input.is_open()) {
        cout << "无法打开文件" << endl;
        return 1; // 返回非零值表示程序异常结束
    }
    int n;
    input >> n;
    vector<vector<int>> points(n, vector<int>(2));
    for (int i = 0; i < n; ++i) {
        input >> points[i][0] >> points[i][1];
    }
    input.close();
#else
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    int n;
    cin >> n;
    vector<vector<int>> points(n, vector<int>(2));
    for (int i = 0; i < n; i++) 
    {
        cin >> points[i][0] >> points[i][1];
    }
#endif

#ifdef log
    // 获取中间时间点
    auto mid = std::chrono::high_resolution_clock::now();
#endif
    int res = Solution().countTrapezoids(points);
    cout << res << endl;

// 获取结束时间点
#ifdef log
    auto end = std::chrono::high_resolution_clock::now();
    // 计算持续时间（以毫秒为单位）
    auto duration_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    auto duration_mid = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid).count();
    std::cout << "函数执行时间: " << duration_seconds << " 毫秒" << std::endl;
    std::cout << "中间函数执行时间: " << duration_mid << " 毫秒" << std::endl;
#endif
    
    return 0;
}