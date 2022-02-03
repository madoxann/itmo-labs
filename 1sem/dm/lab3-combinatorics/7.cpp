#include <bits/stdc++.h>
using namespace std;
//#define int long long
 
vector<int> getReversed(vector<int> vec, int start, int end)
{
    vector<int> ans(vec.begin(), vec.begin() + start);
    for (int i = end; i >= start; i--)
    {
        ans.push_back(vec[i]);
    }
    return ans;
}
 
vector<int> nextPermutation(vector<int> perm, int n)
{
    for (int i = n-2; i >= 0; i--)
    {
        if (perm[i] < perm[i + 1]) {
            int minE = i + 1;
            for (int j = minE; j < n; j++)
            {
                if (perm[j] < perm[minE] && perm[j] > perm[i])
                {
                    minE = j;
                }
            } 
            int tmp = perm[i];
            perm[i] = perm[minE];
            perm[minE] = tmp;
            return getReversed(perm, i+1, n-1);
        }
    }
    return *(new vector<int>(0));
}
 
int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
 
    int n;
    cin >> n;
 
    vector<int> perm;
    for (int i = 1; i <= n ; i++)
    {
        perm.push_back(i);
    }
    while(perm.size() > 0)
    {
        for (auto i : perm)
        {
            cout << i << " ";
        }
        cout << endl;
        perm = nextPermutation(perm, n);
    }
 
    return 0;
}
