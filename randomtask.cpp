#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

typedef long long int LL;

LL m,k;
LL C[70][70];

int init()
{
    for(int i=0;i<70;i++)
        C[i][0]=C[i][i]=1LL;
    for(int i=2;i<70;i++)
    {
        for(int j=1;j<i;j++)
        {
            C[i][j]=C[i-1][j-1]+C[i-1][j];
        }
    }
}

LL get_sum_one(LL x)
{
    LL ret=0;
    int cur=0;
    for(int i=63;i>=0;i--)
    {
        if(x&(1LL<<i))
        {
            if(k-cur>=0) ret+=C[i][k-cur];
            cur++;
        }
    }
    if(cur==k) ret++;
    return ret;
}

LL ck(LL x)
{
    LL sum0=get_sum_one(2*x);
    LL sum1=get_sum_one(x);
    return sum0-sum1;
}

int main()
{
    init();
    LL x;
    cin>>m>>k;
    LL low=1LL,high=1000000000000000000LL,mid,ans=-1;
    while(low<=high)
    {
        mid=(low+high)/2LL;
        LL t=ck(mid);
        if(t>=m) high=mid-1;
        else low=mid+1;
        if(t==m)
        {
            ans=mid;
            break;
        }
    }
    cout<<ans<<endl;
    return 0;
}