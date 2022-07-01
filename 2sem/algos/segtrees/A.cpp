#include <bits/stdc++.h>
using namespace std;
#define int long long

int n;

void update(int* &tree, int p, int value)
{
    tree[p += n] = value;
    while (p /= 2) tree[p] = tree[p * 2] + tree[p * 2 + 1];
}

int query(int* tree, int l, int r)
{
    int res = 0;
    l += n;
    r += n;
    while (l < r)
    {
        if (l % 2 == 1) res += tree[l++];
        l /= 2; 
        if (r % 2 == 1) res += tree[--r];
        r /= 2;
    }
    return res;
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int m;
    cin >> n >> m;

    int* tree = new int[n*4];

    for (int i = 0; i < n; ++i) cin >> tree[i + n];
    for (int i = n - 1; i > 0; i--) tree[i] = tree[i * 2] + tree[i * 2 + 1];

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
            cout << query(tree, l, r) << '\n';
        }
    }

    return 0;
}