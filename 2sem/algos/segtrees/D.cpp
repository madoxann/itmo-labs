#include <bits/stdc++.h>
using namespace std;
#define int long long

int n;

struct Node
{
    int amount;
};

void update(Node *&tree, int p)
{
    tree[p += n].amount != 1;
    while (p /= 2)
    {
        tree[p].amount = tree[p * 2].amount + tree[p * 2 + 1].amount;
    }
}

// int query(Node *tree, int current, int tl, int tr, int k)
// {
//     if (tl == tr)
//         return tl;
//     int tm = (tr + tl) / 2;
//     if (tree[current * 2].amount >= k)
//         return query(tree, current * 2, tl, tm, k);
//     return query(tree, current * 2 + 1, tm + 1, tr, k - tree[current * 2].amount);
// }

int find_kth (Node* t, int v, int tl, int tr, int k) {
	if (k > t[v].amount)
		return -1;
	if (tl == tr)
		return tl;
	int tm = (tl + tr) / 2;
	if (t[v*2].amount >= k)
		return find_kth (t, v*2, tl, tm, k);
	else
		return find_kth (t, v*2+1, tm+1, tr, k - t[v*2].amount);
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int m;
    cin >> n >> m;

    Node *tree = new Node[n * 4];
    for (int i = 0; i < n * 4; i++)
        tree[i].amount = 0;

    for (int i = 0; i < n; ++i)
    {
        cin >> tree[i + n].amount;
    }
    for (int i = n - 1; i > 0; i--)
        tree[i].amount = tree[i * 2].amount + tree[i * 2 + 1].amount;

    //for (int i = 0; i < 4*n; i++) cout << tree[i].amount << '\n';
    for (int i = 0; i < m; i++)
    {
        int opCode;
        cin >> opCode;
        if (opCode == 1)
        {
            int j;
            cin >> j;
            update(tree, j);
        }
        else
        {
            int k;
            cin >> k;
            cout << find_kth(tree, 1, 0, n-1, k+1) << '\n';
        }
    }

    return 0;
}