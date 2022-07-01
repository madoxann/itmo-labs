#include <bits/stdc++.h>
using namespace std;
#define int long long

int n;

struct Node
{
    int min;
    int minCnt;
};

Node recalc(Node l, Node r)
{
    Node ret;
    ret.min = min(l.min, r.min);
    if (l.min == r.min)
    {
        ret.minCnt = l.minCnt + r.minCnt;
        return ret;
    }
    if (ret.min == l.min)
    {
        ret.minCnt = l.minCnt;
        return ret;
    }
    ret.minCnt = r.minCnt;
    return ret;
}

void update(Node* &tree, int p, int value)
{
    tree[p += n].min = value;
    while (p /= 2) 
    {
        // tree[p] = tree[p * 2] + tree[p * 2 + 1];
        tree[p] = recalc(tree[p * 2], tree[p * 2 + 1]);
    }
}

Node query(Node* tree, int l, int r)
{
    Node resL, resR;
    resL.min = INT_MAX;
    resL.minCnt = 0;
    resR.min = INT_MAX;
    resR.minCnt = 0;
    l += n;
    r += n;
    while (l < r)
    {
        if (l % 2 == 1) resL = recalc(resL, tree[l++]);
        l /= 2; 
        if (r % 2 == 1) resR = recalc(resR, tree[--r]);
        r /= 2;
    }
    return recalc(resL, resR);
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int m;
    cin >> n >> m;

    Node* tree = new Node[n*4];
    for (int i = 0; i < n*4; i++)
    {
        tree[i].min = INT_MAX;
        tree[i].minCnt = 0;
    }

    for (int i = 0; i < n; ++i) 
    {
        cin >> tree[i + n].min;
        tree[i+n].minCnt = 1;
    }
    for (int i = n - 1; i > 0; i--) 
    {
        tree[i] = recalc(tree[i * 2], tree[i * 2 + 1]);
    }

    for (int i = 0; i < m; i++)
    {
        int opCode;
        cin >> opCode;
        if (opCode == 1)
        {
            int i, val;
            cin >> i >> val;
            update(tree, i, val);
        }
        else
        {
            int l, r;
            cin >> l >> r;
            Node ret = query(tree, l, r); 
            cout << ret.min << " " << ret.minCnt << '\n';
        }
    }

    return 0;
}