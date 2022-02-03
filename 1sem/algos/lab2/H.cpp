#include <bits/stdc++.h>
using namespace std;
//#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    string str1, str2;
    cin >> str1 >> str2;

    int l1 = str1.size() + 1, l2 = str2.size() + 1;
    vector<vector<int>> dp(l1, vector<int>(l2,0));

    dp[0][0] = 0;
    for (int i = 0; i < l1; i++)
    {
        dp[i][0] = i;
    }
    for (int i = 0; i < l2; i++)
    {
        dp[0][i] = i;
    }

    for (int i = 1; i < l1; i++)
    {
        for (int j = 1; j < l2; j++)
        {
            dp[i][j] = min(dp[i-1][j-1] + (str1[i-1] != str2[j-1]) , min(dp[i-1][j] + 1, dp[i][j-1] + 1));                    
        } 
    }
    cout << dp[l1-1][l2-1];

    return 0;
}
