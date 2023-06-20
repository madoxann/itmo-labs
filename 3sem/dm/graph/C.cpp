#include <bits/stdc++.h>
using namespace std;
//#define int long long

bool cmp(int l, int r)
{
    cout << "1 " << l << " " << r << endl;
    cout.flush();

    string ans;
    cin >> ans;

    return (ans == "YES");
}

int32_t main()
{
    int n;
    cin >> n;

    vector<int> arr;
    for (int i = 1; i < n + 1; i++) arr.push_back(i);

    stable_sort(arr.begin(), arr.end(), cmp);

    cout << "0 ";
    for (int i : arr) cout << i << " ";
    cout.flush();

    return 0;
}