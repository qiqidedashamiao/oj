#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <map>
#include <string>
#include <fstream>
using namespace std;
int main()
{
    int N, K;
    
    /**
     * 随机生成N*K个正整数，并将其写入文件circle.txt，每行10个数，用空格隔开
     */
    //cin >> N >> K;
    // vector<vector<int> > value(N, vector<int>(K));
    // vector<map<string, int>> nums(N);
    // ofstream out("circle.txt");
    // if (!out.is_open())
    // {
    //     cout << "文件打开失败" << endl;
    //     return 0;
    // }
    // out << N << " " << K << endl;
    // for (int i = 0; i < N; i++)
    // {
    //     for (int j = 0; j < K; j++)
    //     {
    //         out << rand() % (N*K) << " ";
    //     }
    //     out << endl;
    // }
    // out.close();
    // return 0;

    /**
     * 目的：从文件circle.txt里读取数据，判断N个环形队列是否相同
     * 文件内容格式：第一行， 一个整数 N 一个整数 K；接下来N行，每一行K个整数，用空格隔开
     */
    // ifstream in("circle1.txt");
    // if (!in.is_open())
    // {
    //     cout << "文件打开失败" << endl;
    //     return 0;
    // }
    // cout << "open success" << endl;
    // in >> N >> K;
    // cout << "N=" << N << " K=" << K << endl;
    // vector<vector<int> > value(N, vector<int>(K));
    // map<int, vector<vector<int>>> nums;
    // for (int i = 0; i < N; i++)
    // {
    //     for (int j = 0; j < K; j++)
    //     {
    //         in >> value[i][j];
    //     }
    // }
    // cout << "read success" << endl;

    //从命令读取N*K个整数
    cin >> N >> K;
    vector<vector<int> > value(N, vector<int>(K));
    map<int, vector<vector<int>>> nums;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < K; j++)
        {
            cin >> value[i][j];
        }
    }

    int res = 0;
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < N; i++)
    {
        //vector<int> temp(K);
        // for (int j = 0; j < K; j++)
        // {
        //     cin >> temp[j];
        // }
        // if (res == 1)
        // {
        //     continue;
        // }
        vector<int> &temp = value[i];
        
        //获取最小值索引集合，兼容出现多个重复的最小值
        vector<int> minIndices(10);
        minIndices[0] = 0;
        int index = 1;
        for (int j = 1; j < K; j++)
        {
            if (temp[j] < temp[minIndices[0]])
            {
                index = 0;
                minIndices[index++] = j;
            }
            else if (temp[j] == temp[minIndices[0]])
            {
                minIndices[index++] = j;
            }
        }
        //获取历史数据库中和当前最小值相同的环形队列，判断是否相同（包含正反两个方向）
        int flag = 0;
        if (nums.find(temp[minIndices[0]]) != nums.end())
        {
            vector<vector<int>> &comp = nums[temp[minIndices[0]]];
            for (int j = 0; j < comp.size(); j++)
            {
                for (int k = 0; k < index; ++k)
                {
                    int curIndex = minIndices[k];
                    int lastIndex = 0;
                    while (temp[curIndex++] == comp[j][lastIndex++] && lastIndex < K)
                    {
                        if (curIndex == K)
                        {
                            curIndex = 0;
                        }
                    }
                    if (lastIndex == K)
                    {
                        flag = 1;
                        break;
                    }
                    curIndex = minIndices[k];
                    lastIndex = 0;
                    while (temp[curIndex--] == comp[j][lastIndex++] && lastIndex < K)
                    {
                        if (curIndex == -1)
                        {
                            curIndex = K - 1;
                        }
                    }
                    if (lastIndex == K)
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 1)
                {
                    break;
                }
            }
        }
        
        if (flag == 1)
        {
            res = 1;
            break;
        }

        //将当前环形队列存入历史数据库
        int curIndex = minIndices[0];
        vector<int> wait(K);
        int k = 0;
        for (int j = curIndex; j < K; j++)
        {
            wait[k++] = temp[j];
        }
        for (int j = 0; j < curIndex; j++)
        {
            wait[k++] = temp[j];
        }
        nums[temp[curIndex]].push_back(wait);
    }

    cout << res << endl;
    auto end = chrono::steady_clock::now();
    // cout << "函数运行时间：" << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

    return 0;
}