#include <bits/stdc++.h>
using namespace std;
//#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n;
    cin >> n;
    
    vector<unordered_set<int>> graph(n, unordered_set<int>());

    for (int i = 1; i < n; i++) 
    {
        int l, r;
        cin >> l >> r;

        graph[--l].emplace(--r);
        graph[r].emplace(l);
    }

    set<int> leaves;

    for (int i = 0; i < n; i++)
        if (graph[i].size() == 1)
            leaves.emplace(i);
    
    int current_leaf;
    for (;n > 2; leaves.extract(current_leaf))
    {
        current_leaf = *leaves.begin();
        int code = *graph[current_leaf].begin();

        cout << code + 1 << " ";

        graph[code].extract(current_leaf);
        if (graph[code].size() == 1) leaves.emplace(code);
        
        n--;
    }    

    return 0;
}