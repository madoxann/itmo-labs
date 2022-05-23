#include <bits/stdc++.h>
using namespace std;
#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    ifstream in; 
    ofstream out;
    
    int n;
    in.open("input.txt", ios::in);
    in >> n;

    vector<int> dp(n,0);

    dp[0] = 1;
    for (int i = 1; i < n; i++)
    {
        dp[i] = dp[i-1];
        if (i > 1)
        {
            dp[i] += dp[i-2];
        }
    }    

    out.open("output.txt");
    out << dp[n-1];   

    return 0;
}
