#include <bits/stdc++.h>
using namespace std;
//#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n;
    cin >> n;
    vector<int> arr(n, 0);
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }
    for (int i = 0; i < n; i++)
    {
        if (2 * i + 1 < arr.size())
        {
            if (arr[i] <=  arr[2 * i + 1])
            {
                if (2 * i + 2 < arr.size())
                {
                    if (!(arr[i] <= arr[2 * i + 2]))
                    {
                        cout << "NO" << endl;
                        return 0;
                    }
                }
            }
            else
            {
                cout << "NO" << endl;
                return 0;
            }
        }
    }
    
    cout << "YES" << endl;
    return 0;
}