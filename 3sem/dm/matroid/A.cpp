#include <bits/stdc++.h>
using namespace std;
#define int long long
 
int32_t main()
{
    freopen("schedule.in", "r", stdin);
    freopen("schedule.out", "w", stdout);
     
    int n;
    cin >> n;
 
    set<pair<int, int>> sched;
    for (int i = 0; i < n; i++)
    {
        int dl, w;
        cin >> dl >> w;
        sched.emplace(make_pair(dl, w));
    }
 
    int ans = 0; 
    multiset<int> done;
    for (auto day : sched)
    {
        done.insert(day.second);
        if (done.size() > day.first) 
        {
            ans += (*done.begin());
            done.erase(done.begin());
        }
    }
    cout << ans;
     
    return 0;
}