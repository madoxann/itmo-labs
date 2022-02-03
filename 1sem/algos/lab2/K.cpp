#include <bits/stdc++.h>
using namespace std;
#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n;
    cin >> n;

    vector<int> seqence(n, 0);
    vector<int> dp(n, 0);
    vector<int> previous(n, 0);

    for (int i = 0; i < n; i++)
        cin >> seqence[i];

    int ans = 1, walk = 0;

    for (int i = 0; i < n; i++)
    {
        dp[i] = 1;
        previous[i] = -1;
        for (int j = 0; j < i; j++)
        {
            if (seqence[j] < seqence[i] && dp[j] + 1 > dp[i])
            {
                dp[i] = dp[j] + 1;
                previous[i] = j;
            }
        }
        if (dp[i] > ans)
        {
            walk = i;
            ans = dp[i];
        }
    } 

    vector<int> answer;
    cout << ans << endl;

    while (walk != -1)
    {
        answer.push_back(seqence[walk]);
        walk = previous[walk];
    }

    for (int i = answer.size()-1; i > -1; i--)
        cout << answer[i] << " ";
    cout << endl;

    return 0;
}