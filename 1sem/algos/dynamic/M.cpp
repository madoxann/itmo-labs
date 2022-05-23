#include <bits/stdc++.h>
using namespace std;
//#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n, s;
    cin >> n >> s;

    vector<int> gold(n+1, 0);
    vector<vector<int>> dp(n + 1, vector<int>(s+1,0)); 

    for (int i = 1; i < n + 1; i++)
    {
        cin >> gold[i];
    }

    for (int i = 1; i < n + 1; i++)
    {
        for (int j = 0; j < s+1; j++)
        {
            dp[i][j] = dp[i-1][j];
            if (j >= gold[i] && dp[i-1][j-gold[i]] + gold[i] > dp[i][j])
            {
                dp[i][j] = dp[i-1][j - gold[i]] + gold[i];
            }
            if (dp[i][j] == s)
            {
                cout << "YES" << endl;
                return 0; 
            }
        }
    }

    cout << "NO" << endl;
    return 0;
}