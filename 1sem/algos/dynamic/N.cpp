#include <bits/stdc++.h>
using namespace std;
//#define int long long

vector<int> *gold;
vector<int>* getItems(vector<vector<int>> dp, int items, int weight)
{
    if (!dp[items][weight]) return new vector<int>;
    if (dp[items-1][weight] == dp[items][weight]) return getItems(dp, items - 1, weight);
    vector<int>* ans = getItems(dp, items-1, weight - (*gold)[items]);
    (*ans).push_back((*gold)[items]);
    return ans;
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n, s;
    cin >> n >> s;

    gold = new vector<int>(n+1, 0); 
    vector<vector<int>> dp(n + 1, vector<int>(s+1,0)); 

    for (int i = 1; i < n + 1; i++)
    {
        cin >> (*gold)[i];
    }

    for (int i = 1; i < n + 1; i++)
    {
        for (int j = 0; j < s+1; j++)
        {
            dp[i][j] = dp[i-1][j];
            if (j >= (*gold)[i] && dp[i-1][j-(*gold)[i]] + (*gold)[i] > dp[i][j])
            {
                dp[i][j] = dp[i-1][j - (*gold)[i]] + (*gold)[i];
            }
        }
    }

    cout << dp[n][s] << endl;

    vector<int> items = (*getItems(dp, n, s));
    cout << items.size() << endl;
    for (auto i : items) cout << i << " ";
    cout << endl;

    return 0;
}