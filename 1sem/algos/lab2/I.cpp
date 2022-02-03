#include <bits/stdc++.h>
using namespace std;
//#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n;
    cin >> n;
    vector<int> price(n, 0);
    for (int i = 0; i < n; i++)
        cin >> price[i];
    
    vector<vector<int>> dp(n+1, vector<int>(n+1, INT_MAX-1000));
    int remainingCoupons = 0;
    dp[0][0] = 0;

    for (int i = 1; i < n + 1; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (price[i-1] > 100)
            {
                if (j-1 > 0)
                {
                    dp[i][j] = min(dp[i-1][j-1] + price[i-1], dp[i-1][j+1]);
                }
                else
                {   
                    dp[i][j+1] = dp[i-1][j] + price[i-1];
                    j++;
                }
                
            }
            else
            {
                if (j-1 > 0)
                {
                    dp[i][j] = min(dp[i-1][j] + price[i-1], dp[i-1][j+1]);
                }
                else
                {
                    dp[i][j] = dp[i-1][j] + price[i-1];
                }
            }
        }
    }

    for (int i = 0; i < n+1; i++)
    {
        for (int j = 0; j < n+1; j++)
        {
            cout << dp[i][j] << " ";
        }
        cout << endl;
    }        

    return 0;
}