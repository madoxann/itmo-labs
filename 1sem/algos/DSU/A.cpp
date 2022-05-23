#include <bits/stdc++.h>
using namespace std;
//#define int long long

vector<int> places;

int find (int elem)
{
    if (elem == places[elem]) return elem;
    return places[elem] = find(places[elem]);
}

void unite (int a, int b)
{
    a = find(a);
    b = find(b);
    places[b] = a;
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n, parked = 0;
    cin >> n;

    int preference[n];
    for (int i = 0; i < n; i++) 
    {
        cin >> preference[i];
        places.push_back(i);
    }

    vector<int> ans(n, 0);
    for (auto i : preference)
    {
        i--;
        while (i != find(i))
        {
            i = find(i);
        }
        ans[parked++] = find(i);
        unite((i+1)%n, i);
    }      

    for (auto i : ans) cout << i + 1 << " ";

    return 0;
}