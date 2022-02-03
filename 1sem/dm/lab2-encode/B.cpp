#include <bits/stdc++.h>
using namespace std;
//#define int long long

string cycShift(string inStr)
{
    string temp = inStr.substr(1, inStr.size());
    temp.append(string(1, inStr[0]));
    return temp;
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    string arrIn;
    cin >> arrIn;
    vector<string> allShifts;
    allShifts.push_back(arrIn);
    for (int i = 1; i < arrIn.size(); i++)
    {
        arrIn = cycShift(arrIn);
        allShifts.push_back(arrIn);
    }
    sort(allShifts.begin(), allShifts.end());
    for (string item : allShifts) cout << item[item.size() - 1];
    cout << endl;

    return 0;
}