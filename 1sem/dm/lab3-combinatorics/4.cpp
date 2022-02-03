#include <bits/stdc++.h>
#include <vector>
using namespace std;
//#define int long long
  
int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
  
    int n;
    cin >> n;
    string curr(n,'0') ;
    cout << curr << endl;
    set<string> ans;
    ans.emplace(curr);
    while (true) {
        string prefix(curr.begin() + 1, curr.end());
        if (ans.emplace(prefix + '1').second)
        {
            curr = prefix + '1';
        }
        else
        {
            if (ans.emplace(prefix + '0').second)
            {
                curr = prefix + '0';
            }
            else
            {
                break;
            }
        }
        cout << curr << endl;
    }
  
    return 0;
}
