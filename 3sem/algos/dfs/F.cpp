#include <bits/stdc++.h>
using namespace std;
//#define int long long

map<int, int> assigned_comonents;
set<set<int>> edges_between;
vector<unordered_set<int>> graph, graph_rev;
vector<int> visited;
deque<int> sorted;

int component_ind = 0;

void topsort(int ind)
{
    visited[ind] = 1;

    for (int n : graph[ind])
        if (!visited[n])
            topsort(n);

    sorted.push_front(ind);
}

void mark_component(int ind)
{
    visited[ind] = 1;
    assigned_comonents.emplace(ind, component_ind);

    for (int n : graph_rev[ind])
        if (!visited[n]) mark_component(n);
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n, m;
    cin >> n >> m;
    visited = vector(n, 0);

    graph = graph_rev = vector(n, unordered_set<int>());
    for (int i = 0; i < m; i++)
    {
        int b, e;

        cin >> b >> e;
        graph[--b].emplace(--e);
        graph_rev[e].emplace(b);
    }

    for (int i = 0; i < n; i++)
        if (!visited[i]) topsort(i);

    visited.assign(n, 0);
    for (int next : sorted)
        if (!visited[next])
        {
            mark_component(next);
            component_ind++;
        }
    
    for (int i = 0; i < n; i++)
        for (int n : graph[i])
            if (assigned_comonents[i] != assigned_comonents[n])
                edges_between.emplace(set{assigned_comonents[i], assigned_comonents[n]});

    cout << edges_between.size() << endl;

    return 0;
}