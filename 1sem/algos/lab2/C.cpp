#include <bits/stdc++.h>
using namespace std;
#define int long long

void go(vector<int>& dp, int n)
{
    if (dp.size() > n)
        return;
    dp.push_back(dp[dp.size()-1] + dp[dp.size()-2]);
    go(dp, n);
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    ifstream in;
    ofstream out;

    in.open("input.txt", ios::in);
    int n;
    in >> n;

    vector<int> dp;
    dp.push_back(0);
    dp.push_back(2);
    dp.push_back(3);
    go(dp, n);

    out.open("output.txt");
    out << dp[n];

    return 0;
}