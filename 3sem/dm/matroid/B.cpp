#include <bits/stdc++.h>
using namespace std;
#define int long long

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

struct edge
{
    int b, e, w, n;

    edge(int begin, int end, int wieght, int order)
    {
        b = begin;
        e = end;
        w = wieght;
        n = order;
    }

    friend bool operator<(edge l, edge r)
    {
        return l.w > r.w;
    }
};

int32_t main()
{
    freopen("destroy.in", "r", stdin);
    freopen("destroy.out", "w", stdout);

    int n, m, s/*, ans = 0*/;
    cin >> n >> m >> s;

    vector<edge> edges = vector(m, edge(0,0,0,-1));

    dsu = vector(n, (int)0);
    for (int i = 0; i < n; i++) 
        dsu[i] = i;

    for (int i = 0; i < m; i++)
    {
        int b, e, w;
        cin >> b >> e >> w;

        edges[i] = edge(--b, --e, w, i+1);
    }

    sort(edges.begin(), edges.end());

    unordered_set<int> mst;
    for (int i = 0; i < m; i++)
    {
        if (find(edges[i].b) != find(edges[i].e))
        {
            mst.insert(edges[i].n);
            unite(edges[i].b, edges[i].e);
        }
    }    
        
    vector<pair<int, int>> try_delete;
    for (auto i : edges) 
        if (mst.find(i.n) == mst.end())
            try_delete.push_back(make_pair(i.w, i.n));
    
    sort(try_delete.begin(), try_delete.end());

    set<int> ans;
    for (auto i = try_delete.begin(); i != try_delete.end(); i++)
    {
        if (s >= (*i).first)
        {
            ans.insert((*i).second);
            s -= (*i).first;
        }
    }
        
    cout << ans.size() << endl;
    for (auto i : ans)
        cout << i << " ";

    return 0;
}