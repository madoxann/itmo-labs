#include <bits/stdc++.h>
using namespace std;
//#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    string arrIn;
    cin >> arrIn;
    vector<string> allShifts(arrIn.size(), "");
    for (int i = 0; i < arrIn.size(); i++)
    {
        for (int j = 0; j < arrIn.size(); j++)
        {
            allShifts[j] = arrIn[j] + allShifts[j];
        } 
        sort(allShifts.begin(), allShifts.end());
    }
    cout << allShifts[0];

    return 0;
}
