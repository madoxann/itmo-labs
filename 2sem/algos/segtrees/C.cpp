#include <bits/stdc++.h>
using namespace std;
#define int long long

int n;

struct Node
{
    int ans;
    int whole;
    int suff;
    int pref;
};

Node recalc(Node l, Node r)
{
    Node ret;
    ret.whole = l.whole + r.whole;
    ret.pref = max(l.pref, l.whole + r.pref);
    ret.suff = max(r.suff, r.whole + l.suff);
    ret.ans = max(l.suff + r.pref, max(l.ans, r.ans));
    return ret;
}

Node init(int value)
{
    Node ret;
    ret.whole = value;
    ret.pref = max((int)0, value);
    ret.suff = ret.pref;
    ret.ans = ret.pref;
    return ret;
}

void update(Node *&tree, int p, int value)
{
    tree[p += n] = init(value);
    while (p /= 2)
    {
        tree[p] = recalc(tree[p * 2], tree[p * 2 + 1]);
    }
}

Node query(Node *tree, int l, int r)
{
    Node resL = init(0), resR = init(0);
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

    Node *tree = new Node[n * 4];
    for (int i = 0; i < n * 4; i++) tree[i] = init(0);

    for (int i = 0; i < n; ++i)
    {
        int val; 
        cin >> val;
        tree[i + n] = init(val);
    }
    for (int i = n - 1; i > 0; i--) tree[i] = recalc(tree[i * 2], tree[i * 2 + 1]);

    cout << query(tree, 0, n).ans << '\n';
    for (int i = 0; i < m; i++)
    {
        int ind, val;
        cin >> ind >> val;
        update(tree, ind, val);
        cout << query(tree, 0, n).ans << '\n';
    }

    return 0;
}