#include<cstdio>
#include<iostream>
#include<cstring>
#include<string>
using namespace std;

const int maxn=1000005;
int prime[maxn],vis[maxn],cnt,L,len1,len2,Kt[35];
string s;

void get_prime(){
    memset(vis,1,sizeof(vis));
    for(int i=2;i<=1000000;++i){
        if(vis[i]) prime[cnt++]=i;
        for(int j=0;j<cnt&&i*prime[j]<=1000000;++j){
            vis[i*prime[j]]=0;
            if(i%prime[j]==0) break;
        }
    }
}

bool Mod(int x){
    int M=0;
    for(int i=0;i<len2;++i)
        M=(M*1000+Kt[i])%x;
    return M==0?true:false;
}

int main(){
    get_prime();
    while(cin>>s>>L,(s[0]!='0')||L){
        len1=s.length();
        if(len1%3==1) s="00"+s;
        else if(len1%3==2) s="0"+s;
        len1=s.length();
        len2=len1/3;
        for(int i=0;i<len2;++i)
            Kt[i]=(s[3*i]-'0')*100+(s[3*i+1]-'0')*10+(s[3*i+2]-'0');
        bool flag=true;
        int p=0;
        while(p<cnt&&prime[p]<L){
            if(Mod(prime[p])){
                flag=false;
                printf("BAD %d\n",prime[p]);
                break;
            }
            ++p;
        }
        if(flag)
            printf("GOOD\n");
    }
    return 0;
}