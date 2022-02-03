#include <bits/stdc++.h>
using namespace std;

//#define int long long

int32_t main()
{
    int n;
    vector<int> arr(101, 0);
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int currNum;
        cin >> currNum;
        arr[currNum]++;
    }
    for (int i = 0; i < 101; i++)
    {
        if (arr[i] > 0)
        {
            for (int j = 0; j < arr[i]; j++)
            {
                cout << i << " ";
            }
        } 
    }
    return 0;
}