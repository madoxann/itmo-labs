#include <bits/stdc++.h>
using namespace std;
//#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n, k, b1, m, a1;
    cin >> n >> a1 >> k >> b1 >> m;

    vector<int> seqence(n, 0);
    seqence[0] = a1;
    for(int i = 1; i < n; i++){
        seqence[i] = (k * seqence[i - 1] + b1) % m;
    }

    vector<int> dp(n, INT_MAX);
    dp[0] = INT_MIN;

    for (int i = 0; i < n; i++)
    {
        int j = int(upper_bound (dp.begin(), dp.begin() + n + 1, seqence[i]) - dp.begin());
            if (dp[j-1] < seqence[i] && seqence[i] < dp[j])
                dp[j] = seqence[i];
    }
    for (int i = 0; i < n; i++)
    {
        if (dp[i] == INT_MAX)
        {
            cout << i - 1 << endl;
            return 0;
        }
    }
    cout << n << endl;
    return 0;
}