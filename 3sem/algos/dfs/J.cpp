#include <bits/stdc++.h>
using namespace std;
// #define int long long

struct edge
{
    int b;
    int e;
    int w;

    edge(int begin, int end, int wieght)
    {
        b = begin;
        e = end;
        w = wieght;
    }

    friend bool operator<(edge l, edge r)
    {
        return l.w < r.w;
    }
};

vector<int> dsu;

int find (int elem)
{
    if (elem == dsu[elem]) return elem;
    return dsu[elem] = find(dsu[elem]);
}

void unite (int a, int b)
{
    a = find(a);
    b = find(b);
    dsu[b] = a;
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n, m; 
    long long ans = 0;
    cin >> n >> m;

    vector<edge> edges = vector(m, edge(0,0,0));

    dsu = vector(n, 0);
    for (int i = 0; i < n; i++) 
        dsu[i] = i;

    for (int i = 0; i < m; i++)
    {
        int b, e, w;
        cin >> b >> e >> w;

        edges[i] = edge(--b, --e, w);
    }

    sort(edges.begin(), edges.end());

    for (int i = 0; i < m; i++)
        if (find(edges[i].b) != find(edges[i].e))
        {
            ans += edges[i].w;
            unite(edges[i].b, edges[i].e);
        }

    cout << ans;

    return 0;
}