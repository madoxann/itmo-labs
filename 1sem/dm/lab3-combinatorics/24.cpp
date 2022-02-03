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
    return *(new vector<int>(n, 0));
}
 
vector<int> perviousPermutation(vector<int> perm, int n)
{
    for (int i = n-2; i >= 0; i--)
    {
        if (perm[i] > perm[i + 1]) {
            int maxE = i + 1;
            for (int j = maxE; j < n; j++)
            {
                if (perm[j] > perm[maxE] && perm[j] < perm[i])
                {
                    maxE = j;
                }
            } 
            int tmp = perm[i];
            perm[i] = perm[maxE];
            perm[maxE] = tmp;
            return getReversed(perm, i+1, n-1);
        }
    }
    return *(new vector<int>(n, 0));
}
 
int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
 
    int n;
    cin >> n;
 
    vector<int> perm(n, 0);
    for (int i = 0; i < n; i++)
    {
        cin >> perm[i];
    }
 
    for (auto i : perviousPermutation(perm, n))
    {
        cout << i << " ";
    }
    cout << endl;
 
    for (auto i : nextPermutation(perm, n))
    {
        cout << i << " ";
    }
    cout << endl;
     
 
    return 0;
}
