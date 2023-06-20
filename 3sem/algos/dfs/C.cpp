#include <bits/stdc++.h>
using namespace std;
//#define int long long

set<int> cutpoints;
vector<unordered_set<int>> graph;
vector<int> visited, dp, tin;

int timer = 0;

void dfs(int ind, int parent_ind = -1) 
{
    visited[ind] = 1;
    dp[ind] = tin[ind] = ++timer;

    int size = 0;

    for (int n : graph[ind]) 
    {
        if (n == parent_ind) continue;
        if (visited[n]) dp[ind] = min(dp[ind], tin[n]);
        else 
        {
            dfs(n, ind);
            dp[ind] = min(dp[ind], dp[n]);
            if (dp[n] >= tin[ind] && parent_ind != -1)
                cutpoints.insert(ind); 
            
            size++;
        }
    }

    if (parent_ind == -1 && size > 1)
        cutpoints.insert(ind); 
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n, m;
    cin >> n >> m;

    graph = vector(n, unordered_set<int>());
    dp = tin = vector(n, INT_MAX);
    visited = vector(n, 0);

    for (int i = 0; i < m; i++)
    {
        int b, e;
        cin >> b >> e;

        graph[--b].emplace(--e);
        graph[e].emplace(b);
    }

    for (int i = 0; i < n; i++)
        if (!visited[i]) dfs(i);

    cout << cutpoints.size() << endl;
    for (int i : cutpoints)
        cout << i + 1 << " ";

    return 0;
}