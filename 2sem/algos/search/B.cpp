#include <bits/stdc++.h>
using namespace std;
#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    // INT_MAX IQ move

    int n;
    cin >> n;

    vector<int> arr(n);
    for (int i = 0; i < n; i++)
    {   
        cin >> arr[i];
    }

    sort(arr.begin(), arr.end());

    cout << n << endl;
    if (n > 0)
    {
        int cnt = 1;
        while (cnt < n)
            cout << arr[cnt-1] << " " << -1 << " " << ++cnt << endl;
        cout << arr[cnt-1] << " " << -1 << " " << -1 << endl;
    }
    cout << 1 << endl;

    return 0;
}