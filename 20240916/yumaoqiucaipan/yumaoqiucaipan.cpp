#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int main()
{
    string s;
    cin >> s;
    int n = s.size();
    int gao = 0, wang = 0;
    for (int i = 0; i < n; i++)
    {
        if (s[i] == 'G')
        {
            gao++;
        }
        else if (s[i] == 'W')
        {
            wang++;
        }
        int ax = max(gao, wang);
        int ay = min(gao, wang);
        int flag = 0;
        if (ax - ay >= 2 && ax == 21)
        {
            flag = 1;
        }
        else if (ax  > 21 && ax - ay == 2)
        {
            flag = 1;
        }
        else if (ax == 30)
        {
            flag = 1;
        }
        if (flag)
        {
            cout << gao << ":" << wang << endl;
            gao = wang = 0;
        }
    }
    if (gao || wang)
    {
        cout << gao << ":" << wang << endl;
    }
    return 0;
}