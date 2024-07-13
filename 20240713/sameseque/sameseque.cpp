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
    cin >> N >> K;
    vector<vector<int> > value(N, vector<int>(K));
    vector<map<string, int>> nums(N);

    
    /**
     * 随机生成1000000个正整数，并将其写入文件circle.txt，每行10个数，用空格隔开
     */
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
    //return 0;
    /**
     * 目的：从文件circle.txt里读取数据，判断N个环形队列是否相同
     * 文件内容格式：第一行， 一个整数 N 一个整数 K；接下来N行，每一行K个整数，用空格隔开
     */
    // ifstream in("circle.txt");
    // if (!in.is_open())
    // {
    //     cout << "文件打开失败" << endl;
    //     return 0;
    // }
    // in >> N >> K;
    
    // for (int i = 0; i < N; i++)
    // {
    //     for (int j = 0; j < K; j++)
    //     {
    //         in >> value[i][j];
    //     }
    // }

    int res = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < K; j++)
        {
            cin >> value[i][j];
        }
    }
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < N; i++)
    {
        //vector<int> temp(K);
        // for (int j = 0; j < K; j++)
        // {
        //     cin >> temp[j];
        //     //cin >> nums[i][j];
        // }
        vector<int> &temp = value[i];
        if (res == 1)
        {
            continue;
        }
        /**
         * 从容器temp的任意位置的索引开始，将temp的元素序列化之后合并成一个string字符串，存入map容器nums[i]中，然后反方向再序列化一遍存入map容器nums[i]中
         */
        for (int j = 0; j < K; j++)
        {
            string str = "";
            for (int k = j; k < K; k++)
            {
                str += to_string(temp[k]);
            }
            for (int k = 0; k < j; k++)
            {
                str += to_string(temp[k]);
            }
            /**
             * 判断str是否存在于容器nums[0]、num[1]......nums[i-1]中，如果存在则res=1并且break；
            */
            int g = 0;
            for (g = 0; g < i; ++g)
            {
                if (nums[g].find(str) != nums[g].end())
                {
                    res = 1;
                    break;
                }
            }
            if (g < i)
            {
                break;
            }

            nums[i][str]++;
            str = "";
            for (int k = j; k >= 0; k--)
            {
                str += to_string(temp[k]);
            }
            for (int k = K - 1; k > j; k--)
            {
                str += to_string(temp[k]);
            }
            nums[i][str]++;
        }
        
    }

    cout << res << endl;
    auto end = chrono::steady_clock::now();
    cout << "函数运行时间：" << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

    return 0;
}