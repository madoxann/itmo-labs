#include <bits/stdc++.h>
using namespace std;
//#define int long long

map<set<int>, int> edges;
set<int> bridges;
vector<unordered_set<int>> graph;
vector<int> visited, dp, tin;

int timer = 0;

void dfs(int ind, int parent_ind = -1) 
{
    visited[ind] = 1;
    dp[ind] = tin[ind] = ++timer;

    for (int n : graph[ind]) 
    {
        if (n == parent_ind) continue;
        if (visited[n]) dp[ind] = min(dp[ind], tin[n]);
        else 
        {
            dfs(n, ind);
            dp[ind] = min(dp[ind], dp[n]);
            if (dp[n] > tin[ind])
                bridges.insert(edges[set{n, ind}]); 
        }
    }
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n, m;
    cin >> n >> m;

    graph = vector(n, unordered_set<int>());
    visited = vector(n, 0);
    dp = tin = vector(n, INT_MAX);

    for (int i = 0; i < m; i++)
    {
        int b, e;
        cin >> b >> e;

        edges.emplace(set{--b, --e}, i + 1);
        graph[b].insert(e);
        graph[e].insert(b);
    }

    for (int i = 0; i < n; i++)
        if (!visited[i]) dfs(i);

    cout << bridges.size() << endl;
    for (int i : bridges)
        cout << i << " ";

    return 0;
}