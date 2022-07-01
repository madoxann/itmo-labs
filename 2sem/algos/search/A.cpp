#include <bits/stdc++.h>
using namespace std;
//#define int long long

struct Node
{
    Node *l = nullptr;
    Node *r = nullptr;
    int val = 0;

    void reset(Node *l, Node *r, int v)
    {
        this->l = l;
        this->r = r;
        this->val = v;
    }
};

bool dfs(Node *tree)
{
    if ((tree->r != nullptr ? tree->val <= tree->r->val : true) 
        && (tree->l != nullptr ? tree->l->val < tree->val : true))
    {
        bool concat = true;
        if (tree->l != nullptr)
            concat &= dfs(tree->l);
        if (tree->r != nullptr)
            concat &= dfs(tree->r);
        return concat;
    }
    return false;
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int n;
    cin >> n;

    vector<Node> tree(n);
    for (int i = 0; i < n; i++)
    {
        int l, r, v;
        cin >> v >> l >> r;

        tree[i].reset(l > -1 ? &tree[l - 1] : nullptr, r > -1 ? &tree[r - 1] : nullptr, v);
    }

    int root;
    cin >> root;
    cout << (dfs(&tree[root - 1]) ? "YES" : "NO") << endl;

    return 0;
}