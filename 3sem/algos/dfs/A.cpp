#include <bits/stdc++.h>
using namespace std;
//#define int long long

enum colors {
    WHITE, GREY, BLACK
};

vector<unordered_set<int>> graph;
vector<colors> acyclic_visited;
vector<int> visited;
deque<int> sorted;

void dfs(int ind)
{
    visited[ind] = 1;

    for (int n : graph[ind])
        if (!visited[n])
            dfs(n);

    sorted.push_front(ind);
}

void isAcyclic(int ind)
{
    acyclic_visited[ind] = GREY;

    for (int n : graph[ind])
    {
        if (acyclic_visited[n] == WHITE) isAcyclic(n);
        if (acyclic_visited[n] == GREY)
        {
            cout << "-1";
            exit(0);
        }
    }

    acyclic_visited[ind] = BLACK;
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n, m;
    cin >> n >> m;

    graph = vector(n, unordered_set<int>());
    visited = vector(n, 0);

    for (int i = 0; i < m; i++)
    {
        int b, e;

        cin >> b >> e;
        graph[--b].insert(--e);
    }

    acyclic_visited = vector(n, WHITE);
    for (int i = 0; i < n; i++)
        if (acyclic_visited[i] == WHITE) isAcyclic(i);

    for (int i = 0; i < n; i++)
        if (!visited[i]) dfs(i);

    for (int i : sorted) cout << ++i << " ";

    return 0;
}