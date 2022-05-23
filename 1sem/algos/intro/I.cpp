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
    stack<int> deadEnd;
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }
    deadEnd.push(arr[0]);
     
    int currWagon = 1, prevOut = 0, currMin = 1, wagonsWent = 0;
    string ans = "1 1\n";

    for (;;) 
    {
        while (deadEnd.top() != currMin && currWagon < n)
        {
            deadEnd.push(arr[currWagon]);
            currWagon++;
            ans += "1 1\n";
        }
        currMin++;

        if (prevOut == deadEnd.top() - 1)
        {
            ans += "2 1\n";
            prevOut = deadEnd.top();
            deadEnd.pop();
            wagonsWent++;
            if (deadEnd.empty() && wagonsWent == n)
            {
                break;
            }
            else if (deadEnd.empty())
            {
                deadEnd.push(arr[currWagon]);
                currWagon++;
                ans += "1 1\n";
            }
        }
        else 
        {
            cout << "0";
            return 0;
        }
    }

    cout << ans; 
    return 0;
}