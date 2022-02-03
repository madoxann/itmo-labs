#include <bits/stdc++.h>
using namespace std;
#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    ifstream in;
    ofstream out;

    int n, m;    
    in.open("knight.in", ios::in);
    //cin >> n >> m;
    in >> n >> m;
    vector<vector<int>> dp(n, vector<int>(m, 0));
    dp[0][0] = 1;
    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < m; j++)
        {
            if (i - 2 > -1)
            {
                dp[i][j] += dp[i-2][j-1];
            }
            if (j - 2 > -1)
            {
                dp[i][j] += dp[i-1][j-2];
            }
        }
    }
    
    out.open("knight.out");
    //cout << dp[n-1][m-1] << endl;
    out << dp[n-1][m-1] << endl;
    /*
    for (auto i : dp)
    {
        for (auto j : i)
        {
            cout << j << " ";
        }
        cout << endl;
    }*/
    return 0;
}