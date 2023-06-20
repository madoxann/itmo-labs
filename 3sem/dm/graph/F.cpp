#include <bits/stdc++.h>
using namespace std;
//#define int long long
 
int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
 
    int n;
    cin >> n;
 
    queue<int> prufer;
    set<int> leaves;
 
    for (int i = 0; i < n; i++)
        leaves.emplace(i);
 
    map<int, int> instances;
    set left_leaves = leaves;
 
    for (int i = 0; i < n - 2; i++)
    {
        int tmp;
 
        cin >> tmp;
        prufer.emplace(--tmp);
        instances[tmp]++;
        leaves.extract(tmp);
    }
 
    int current_leaf;

    while (!prufer.empty())
    {
        current_leaf = *leaves.begin();
        leaves.extract(current_leaf);

        cout << prufer.front() + 1 << " " << current_leaf + 1 << endl;
 
        if (!--instances[prufer.front()])
            leaves.emplace(prufer.front());

        left_leaves.extract(current_leaf);
        prufer.pop();
    }
     
    for (int i : left_leaves) cout << ++i << " ";
    cout << endl;
 
    return 0;
}
