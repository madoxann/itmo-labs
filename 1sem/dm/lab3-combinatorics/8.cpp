#include <bits/stdc++.h>
using namespace std;
//#define int long long
 
vector<int> nextChoose(vector<int> choose, int n, int k)
{
    choose.push_back(n+1);
    for (int i = k-1; i > -1; i--)
    {
        if (choose[i+1] - choose[i] >= 2)
        {
            choose[i]++;
            for (int j = i + 1; j < k; j++)
            {
                choose[j] = choose[j-1] + 1;
            }
            return *(new vector<int>(choose.begin(), choose.end()-1));
        }
    }
    return *(new vector<int>(0));
}
 
int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
 
    int n, k;
    cin >> n >> k;
 
    vector<int> choose;
    for (int i = 1; i <= k ; i++)
    {
        choose.push_back(i);
    }
    while(choose.size() > 0 && n >= k)
    {
        for (auto i : choose)
        {
            cout << i << " ";
        }
        cout << endl;
        choose = nextChoose(choose, n, k);
    }
 
    return 0;
}
