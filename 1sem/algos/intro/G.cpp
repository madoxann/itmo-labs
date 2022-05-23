#include <bits/stdc++.h>
using namespace std;
#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n, m;
    cin >> n >> m;

    vector<int> arr(n, 0);
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }    

    vector<int> prefSum(n + 1, 0);
    prefSum[0] = 0;
    for (int i = 1; i < n + 1 ; i++)
    {
        prefSum[i] = prefSum[i-1] + arr[i-1];
    }

    int l, r;
    for (int i = 0; i < m; i++)
    {
        cin >> l >> r;
        cout << prefSum[r] - prefSum[l-1] << endl;
    }
    return 0;
}