
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <ctime>
#include <time.h>
#include <random>
#include <ctime>

using namespace std;

void writefile(vector<int> &di, vector<int> &wi)
{
    // edges和value写入文件
    ofstream outfile("yongli/test7337.in");
    if (!outfile)
    {
        cout << "open file error" << endl;
        return;
    }
    outfile << di.size() << endl;
    for (int i = 0; i < di.size(); i++)
    {
        outfile << di[i] << " " << wi[i] << endl;
    }
    outfile.close();
}

void generte_rand(vector<int> &random_numbers, int v, bool repeat = true)
{
    
    std::srand(std::time(0));
     // 创建一个随机数生成器
    std::mt19937 generator(std::rand());

     // 创建一个分布，定义随机数的范围
    std::uniform_int_distribution<int> distribution(1,v);

    // 创建一个向量来存储生成的随机数
    
    // 生成随机数
    int random_number = distribution(generator);

    // 生成随机数并存储在向量random_numbers中，生成的随机数不能重复
    for (int i = 0; i < random_numbers.size(); ++i) {
        int random_number = distribution(generator);
        if (!repeat) {
            while (std::find(random_numbers.begin(), random_numbers.end(), random_number) != random_numbers.end()) {
                random_number = distribution(generator);
            }
        }
        random_numbers[i] = random_number;
    }

}

int main() {
    int n = 7337;

    //cin >> n;
    //生成距离随机数
    std::vector<int> di(n);
    generte_rand(di, n*10, false);
    //生成权重随机数
    std::vector<int> wi(n);
    generte_rand(wi, n*100);

    sort(di.begin(), di.end());

    writefile(di, wi);

    return 0;
}