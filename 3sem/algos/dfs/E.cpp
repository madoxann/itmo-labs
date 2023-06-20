#include <bits/stdc++.h>
using namespace std;
//#define int long long

map<set<int>, vector<int>> duplicate_edges; // не опять, а снова
map<set<int>, int> edges;
vector<unordered_set<int>> graph;
vector<int> visited, marked, dp, tin;

int color;
int timer = color = 0;

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
        }
    }
}

inline void mark_edge(int b, int e, int mark)
{
    set<int> tmp = set{b, e};
    marked[edges[tmp]] = mark;
    if (duplicate_edges.find(tmp) != duplicate_edges.end())
        for (int dup_edge : duplicate_edges[tmp])
            marked[dup_edge] = mark;
}

void dfs_mark(int ind, int mark, int parent_ind = -1) 
{
    visited[ind] = 1;
    for (int n : graph[ind])
    {
        if (n == parent_ind) continue;
        if (!visited[n])
        {
            if (dp[n] >= tin[ind])
            {
                mark_edge(ind, n, ++color);
                dfs_mark(n, color, ind);
            }
            else 
            {
                mark_edge(ind, n, mark);
                dfs_mark(n, mark, ind);
            }
        }
        else if (tin[n] < tin[ind])
            mark_edge(ind, n, mark);
    }
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n, m;
    cin >> n >> m;

    visited = vector(n, 0);
    marked = vector(m, 0);
    tin = dp = vector(n, INT_MAX);
    graph = vector(n, unordered_set<int>());

    for (int i = 0; i < m; i++)
    {
        int b, e;
        cin >> b >> e;
        
        if (auto edge_num = edges.find(set{--b, --e}); edge_num == edges.end())
            edges.emplace(set{b, e}, i);
        else duplicate_edges[edge_num->first].push_back(i);

        graph[b].emplace(e);
        graph[e].emplace(b);
    }

    for (int i = 0; i < n; i++)
        if (!visited[i]) dfs(i);

    visited.assign(n, 0);
    for (int i = 0; i < n; i++)
        if (!visited[i]) dfs_mark(i, color);

    cout << color << endl;
    for (int i : marked) cout << i << " ";

    return 0;
}