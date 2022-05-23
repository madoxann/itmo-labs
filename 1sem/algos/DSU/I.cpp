#include <bits/stdc++.h>
using namespace std;
#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n = 0, maxNum = 0;
    cin >> n;

    vector<int> toFactor(n,0);
    for (int i = 0; i < n; i++) 
    {
        cin >> toFactor[i];
        maxNum = max(maxNum, toFactor[i]);
    }

    vector<int> primes;
    vector<int> mp(maxNum + 1, -1);

    for (int i = 2; i <= maxNum; i++)
    {
        if (mp[i] == -1)
        {
            primes.push_back(i);
            mp[i] = i;
        }
        for (auto p : primes)
        {
            if (p > mp[i] || i * p > maxNum) break;
            mp[i * p] = p;
        }
    }

    for (auto i : toFactor)
    {
        if (mp[i] == i) cout << i << endl;
        else
        {
            vector<int> ans;
            while (i > 1)
            {
                ans.push_back(mp[i]);
                i /= mp[i];
            }
            
            for (auto j : ans)
            {
                cout << j << " ";
            }
            cout << endl;
        }
    }

    return 0;
}