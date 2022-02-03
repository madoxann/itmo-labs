#include <bits/stdc++.h>
#include <vector>
using namespace std;
//#define int long long
  
vector<vector<int>> getVectors(vector<vector<int>> ans, vector<int> currVector, int len)
{
    if (currVector.size() == len)
    {
        ans.push_back(currVector);
        return ans;
    }
    vector<int> tmp = currVector;
    //cout << tmp.size();
    if (tmp.size() > 0 && tmp[tmp.size()-1] == 1)
    {
        tmp.push_back(0);
        return getVectors(ans, tmp, len);
    }
    currVector.push_back(0);
    tmp.push_back(1);
    ans = getVectors(ans, currVector, len);
    ans = getVectors(ans, tmp, len);
    return ans;
} 
  
  
int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
  
    int n;
    cin >> n;
    vector<vector<int>> ans(0, vector<int>(0,0));
    ans = getVectors(ans, *(new vector<int>), n);
    cout << ans.size() << endl;
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
