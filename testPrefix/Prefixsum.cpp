#include<iostream>
#include<vector>
#include"Prefixsum.hpp"
using namespace std;

//算法1：
//一维 ： 前缀和数组的求法
//arr  0 1 2 3 4 5 6 7 8 9 10
//      /|/|/|/|/|/|/|/|/|/|
//dp   0 1 2 3 4 5 6 7 8 9 10
//使用前缀和：dp[r] - dp[l - 1]
void PrefixSum_singledimension()
{
    int n = 0; int q = 0;
    cin >> n >> q;
    vector<int> arr(n + 1, 0);
    vector<long long int> dp(n + 1, 0);
    //将数据读入到arr中
    for (int i = 1; i <= n; i++) cin >> arr[i];
    //将数据读入到dp中
    for (int i = 0; i <= n; i++) dp[i] = dp[i - 1] + arr[i];
    //直接读取dp数组
    for (int i = 0; i < q; i++)
    {
        int l = 0; int r = 0;
        cin >> l >> r;
        cout << dp[r] - dp[l - 1] << endl;
    }
}

//算法2：
//二维 ： 前缀和数组的求法
//                   y1      y2
    //------------------------
    //|              |       |
    //|              |       |
    //|     A        |   B   |
    //|              |       |
  //x1|--------------|-------|
    //|     C        |   D   |
  //x2---------------|-------|
//二维数组:dp如何存储，dp[i][j]表示从(1,1)到(i,j)的和，dp[i][j] = dp[i-1][j] + dp[i][j-1] - dp[i-1][j-1] + arr[i][j]
//等价于B+C-A+D
//使用前缀和：dp[r][r] - dp[l-1][r] - dp[r][l-1] + dp[l-1][l-1]
//等价于D+A-B-C
void PrefixSum_twodimension()
{
    int n = 0; int m = 0; int q = 0;
    cin >> n >> m >> q;
    vector<vector<long long int> > arr(n + 1, vector<long long int>(m + 1));//居然还可以这么初始化
    vector<vector<long long int> > dp(n + 1, vector<long long int>(m + 1));//这里初始化的方式要记住
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            int tmp = 0; cin >> tmp;
            arr[i][j] = tmp;
        }
    }
    //将数据放入到dp数组
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1] - dp[i - 1][j - 1] + arr[i][j];
        }
    }
    //使用dp数组
    for (int i = 0; i < q; i++)
    {
        int x1 = 0; int x2 = 0;
        int y1 = 0; int y2 = 0;
        cin >> x1 >> y1 >> x2 >> y2;
        cout << dp[x2][y2] - dp[x2][y1 - 1] - dp[x1 - 1][y2] + dp[x1 - 1][y1 - 1] << endl;
    }
}











