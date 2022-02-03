#include <bits/stdc++.h>
using namespace std;
//#define int long long

void getVectors(vector<vector<int>> ans, vector<int> currVector, int len)
{
    if (currVector.size() == len)
    {
        for (auto i : currVector) cout << i;
        cout << endl;
    }
    else
    {
        vector<int> tmp = currVector;
        currVector.push_back(0);
        tmp.push_back(1);
        getVectors(ans, currVector, len);
        getVectors(ans, tmp, len);
    }
} 
  
int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
  
    int n;
    cin >> n;
    vector<vector<int>> ans(0, vector<int>(0,0));
    getVectors(ans, *(new vector<int>), n);
    return 0;
}
