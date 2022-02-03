#include <bits/stdc++.h>
#include <vector>
using namespace std;
//#define int long long
 
std::vector<vector<int>> getSymCode(vector<vector<int>> curr, int len)
{
    if (curr[0].size() == len)
        return curr;
    curr.resize(curr.size() * 2);
    int cnt = 1;
    for (int i = curr.size()/2; i < curr.size(); i++)
    {
        curr[i] = curr[curr.size()/2-cnt];
        cnt++;
    }
    for (int i = 0; i < curr.size(); i++)
    {
        if (i < curr.size()/2)
        {
            curr[i].emplace(curr[i].begin(), 0);
            //for (auto i : curr[i]) cout << i;
            //cout << endl;
        }
        else {
            curr[i].emplace(curr[i].begin(), 1);
        }
    }
     
    curr = getSymCode(curr, len);
    return curr;
}
 
int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
 
    int n;
    cin >> n;
    vector<vector<int>> ans = getSymCode(*(new vector<vector<int>>{*(new vector<int>(1,0)), *(new vector<int>(1,1))}), n);
    for (auto i : ans)
    {
        for (auto j : i)
        {
            cout << j;
        }
        cout << endl;
    }
 
 
    return 0;
}
