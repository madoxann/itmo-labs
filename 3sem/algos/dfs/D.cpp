#include <bits/stdc++.h>
using namespace std;
//#define int long long

map<set<int>, int> edges; 
set<int> bridges;
unordered_set<int> duplicate_edges; // да, да, отсутствие кратныx ребер никто не обещал))
vector<unordered_set<int>> graph;
vector<int> visited, marked, dp, tin;

int color;
int timer = color = 0;

void dfs_bridges(int ind, int parent_ind = -1) 
{
    visited[ind] = 1;
    dp[ind] = tin[ind] = ++timer;

    for (int n : graph[ind]) 
    {
        if (n == parent_ind) continue;
        if (visited[n]) dp[ind] = min(dp[ind], tin[n]);
        else 
        {
            dfs_bridges(n, ind);
            dp[ind] = min(dp[ind], dp[n]);
            if (dp[n] > tin[ind] && duplicate_edges.find(edges[set{n, ind}]) == duplicate_edges.end())
                bridges.emplace(edges[set{n, ind}]);
        }
    }
}

void dfs_mark(int ind, int mark) 
{
    marked[ind] = mark;
    for (int n : graph[ind])
        if (!marked[n])
            if (bridges.find(edges[set{n, ind}]) != bridges.end())
                dfs_mark(n, ++color);
            else dfs_mark(n, mark);
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n, m;
    cin >> n >> m;

    visited = marked = vector(n, 0);
    dp = tin = vector(n, INT_MAX);
    graph = vector(n, unordered_set<int>());

    for (int i = 0; i < m; i++)
    {
        int b, e;
        cin >> b >> e;

        if (auto edge_num = edges.find(set{--b, --e}); edge_num == edges.end())
            edges.emplace(set{b, e}, i + 1);
        else duplicate_edges.insert(edge_num->second);

        graph[b].emplace(e);
        graph[e].emplace(b);
    }

    for (int i = 0; i < n; i++)
        if (!visited[i]) dfs_bridges(i);

    for (int i = 0; i < n; i++)
        if (!marked[i]) dfs_mark(i, ++color);

    cout << color << endl;
    for (int i : marked) cout << i << " ";

    return 0;
}