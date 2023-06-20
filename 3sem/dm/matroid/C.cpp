#include <bits/stdc++.h>
using namespace std;
// #define int long long

map<int, vector<int>> graph;
vector<int> visited, matched;

int dfs(int v)
{
    if (visited[v]) return 0;
    visited[v] = 1;

    for (int next : graph[v])
    {
        if (matched[next] == -1 || dfs(matched[next]))
        {
            matched[next] = v;
            matched[v] = next;
            return 1;
        }
    }

    return 0;
}

int32_t main()
{
    freopen("matching.in", "r", stdin);
    freopen("matching.out", "w", stdout);

    int n;
    cin >> n;

    vector<pair<int, int>> left_weight(n, make_pair(-1, 0)); // enumarate left 0...n-1, right n...2n-1
    matched.assign(2 * n, (int)-1);

    for (int i = 0; i < n; i++)
    {
        int w;
        cin >> w;

        left_weight[i] = make_pair(w, i);
    }

    for (int i = 0; i < n; i++)
    {
        int n_right;
        cin >> n_right;

        for (int j = 0; j < n_right; j++)
        {
            int v;
            cin >> v;

            graph[i].push_back(v += n - 1);
            graph[v].push_back(i);
        }
    }

    sort(left_weight.begin(), left_weight.end(), [](auto l, auto r) -> bool { return l.first > r.first; });
    for (auto i : left_weight)
    {
        visited.assign(2 * n, (int)0);
        dfs(i.second);
    }

    for (int i = 0; i < n; i++)
        cout << (matched[i] < 0 ? 0 : matched[i] - n + 1) << " ";

    return 0;
}