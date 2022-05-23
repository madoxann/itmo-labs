#include <bits/stdc++.h>
using namespace std;
//#define int long long
pair<int, int> findMax(vector<int> arr, int begin, int k, int n) 
{
    int max = INT_MIN, maxInd;
    int end;
    if (begin + k > n)
        end = n;
    else 
        end = begin + k;
    for (int i = begin - 1; i < end; i++)
    {
        if (arr[i] > max)
        {
            maxInd = i;
            max = arr[i];
            if (max > 0) return make_pair(maxInd + 1, max);
        }
    }
    return make_pair(maxInd + 1, max);
}


int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    ifstream in;
    ofstream out;

    int n, k;
    in.open("input.txt", ios::in);
    in >> n >> k;
    vector<int> value(n, 0);
    value[0] = 0;
    value[n-1] = 0;
    
    for (int i = 1; i < n-1; i++)
    {
        in >> value[i];
    }

    vector<int> dp;
    int val = 0, i = 0;
    while (i < n-1)
    {
        pair<int, int> nextMax = findMax(value, i + 1, k-1, n);
        dp.push_back(nextMax.first);
        val += nextMax.second;
        i = nextMax.first;
    }

    int jumps = dp.size();
    out.open("output.txt");
    out << val << endl;

    vector<int> ans;
    if (dp[0] != 1)
        ans.push_back(1);
    for (auto i : dp)
    {
        ans.push_back(i);
    }
    if (dp[dp.size() - 1] != n)
        ans.push_back(n);
    
    out << ans.size()-1 << endl;
    for (auto i : ans)
        out << i << " ";
    out << endl;

    return 0;
}