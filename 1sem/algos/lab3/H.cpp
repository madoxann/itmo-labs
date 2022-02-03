#include <bits/stdc++.h>
using namespace std;
//#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int num;
    cin >> num;
    
    vector<int> ans;
    int del = 2;

    while (num > 1)
    {
        if (num % del == 0)
        {
            num /= del;
            ans.push_back(del);
        }
        else
        {
            del++;
        }
    }

    for (auto i : ans)
    {
        cout << i << " ";
    }

    return 0;
}