#include <bits/stdc++.h>
using namespace std;
//#define int long long

int Euclid(int a, int b)
{
    while (a != 0 && b != 0)
    {
        if (a > b) a %= b;
        else b %= a;
    }
    return a+b;
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n;
    cin >> n;

    vector<int> toGCD(n,0);
    for (int i = 0; i < n; i++)
    {
        cin >> toGCD[i];
    }

    if (n == 1) 
    {
        cout << toGCD[0] << endl;
        return 0;
    }

    int ans = Euclid(toGCD[0], toGCD[1]);
    for (int i = 2; i < n; i++)
    {
        ans = Euclid(ans, toGCD[i]);
    }

    cout << ans << endl;

    return 0;
}