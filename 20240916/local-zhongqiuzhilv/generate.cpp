
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <ctime>
#include <time.h>

using namespace std;

void parse_edges(vector<vector<int> > &edges, std::string &s)
{
    // vector<vector<int> > edges;
    // s格式为[[0,2],[2,1]]，解析成0,2;2,1
    for (int i = 0; i < s.size(); i++)
    {
        // cout << "i:" << i << endl;
        if (s[i] == '[' || s[i] == ',' || s[i] == ']')
        {
            continue;
        }
        else
        {
            
            vector<int> temp(2);
            int index = s.find(',', i);
            if (index == -1)
            {
                break;
            }
            string sub = s.substr(i, index - i);
            // cout << "sub:" << sub << endl;
            temp[0] = atoi(sub.c_str());
           
            int index2 = s.find(']', i);
            sub = s.substr(index + 1, index2 - index - 1);
            // cout << "sub:" << sub << endl;
            temp[1] = atoi(sub.c_str());
            // int value1 = stoi(s.substr(index + 1, index2 - index - 1));
            // cout << temp[0] << " " << temp[1] << endl;
            if ( i >= index2)
            {
                cout << "i:" << i << " index:" << index << endl;
                cout << s[i] << endl;
                break;
            }
            // cout << "index2:" << index2 << endl;
            i = index2;
            edges.push_back(temp);
        }
    }
}

void parse_value(vector<int> &value, std::string &s1)
{
    // std::string s1;
    // cin >> s1;
    // // cout << s1 << endl;
    // vector<int> value;
    for (int i = 0; i < s1.size(); i++)
    {
        if (s1[i] == '[' || s1[i] == ']' || s1[i] == ',')
        {
            continue;
        }
        else
        {
            int index = s1.find(',', i);
            if (index == string::npos)
            {
                index = s1.find(']', i);
            }
            string sub = s1.substr(i, index - i);
            // cout << "sub:" << sub << endl;
            value.push_back(atoi(sub.c_str()));
            i = index;
        }
    }
}

void readfile(vector<int> &value, vector<vector<int> > &edges)
{
    std::ifstream file("shili5-4000");
    // 检查文件是否成功打开
    if (!file.is_open())
    {
        std::cerr << "无法打开文件: shili1000" << std::endl;
        return ; // 返回非零值表示程序异常结束
    }

    // 读取文件内容
    // vector<vector<int> > edges;
    std::string line;
    std::getline(file, line);
    // cout << line << endl;
    parse_edges(edges, line);
    std::getline(file, line);
    // cout << line << endl;
    parse_value(value,line);
    // while (std::getline(file, line))
    // {
    //     // 打印每一行的内容
    //     std::cout << line << std::endl;
    // }
    cout << "edge:" << edges.size() << endl;
    cout << "value:" << value.size() << endl;
    // cout << edges[edges.size()-1][0] << " " << edges[edges.size()-1][1] << endl;

    // 关闭文件
    file.close();

    // edges和value写入文件
    ofstream outfile("yongli/in5");
    if (!outfile)
    {
        cout << "open file error" << endl;
        return;
    }
    outfile << value.size() << " " << edges.size() << endl;
    for (int i = 0; i < value.size(); i++)
    {
        if (i == value.size() - 1)
        {
            outfile << value[i] << endl;
        }
        else
        {
            outfile << value[i] << " ";
        }
    }
    // outfile << endl;
    for (int i = 0; i < edges.size(); i++)
    {
        outfile << edges[i][0] << " " << edges[i][1] << endl;
    }
    outfile.close();
}

int main() {
    int n, m;
    // cin >> n >> m;
    // vector<int> happiness(n);

    vector<int> value;
    vector<vector<int> > edges;
    
    readfile(value, edges);
    n = value.size();
    m = edges.size();



    return 0;
}