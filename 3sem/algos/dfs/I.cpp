#include <bits/stdc++.h>
using namespace std;
// #define int long long

#define get_distance(l, r) (sqrt(((l).first - (r).first) * ((l).first - (r).first) + ((l).second - (r).second) * ((l).second - (r).second)))

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    double ans = 0;
    int n = 0; 

    cin >> n;

    vector<pair<int, int>> points = vector(n, make_pair(0, 0));
    unordered_set<int> unconnected;

    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;
        points[i] = make_pair(x, y);
        unconnected.insert(i);
    }

    if (n == 1) 
    {
        printf("%f", 0);
        return 0;
    }

    vector<double> known_distance = vector(n, DBL_MAX);

    unconnected.extract(0);
    for (int i = 1; i < n; i++)
        known_distance[i] = get_distance(points[0], points[i]);

    int curr_point;
    for (;!unconnected.empty(); unconnected.extract(curr_point))
    {
        curr_point = *unconnected.begin();

        for (int pt : unconnected)
            if (known_distance[curr_point] > known_distance[pt]) 
                curr_point = pt;

        ans += known_distance[curr_point];

        for (int pt : unconnected)
            if (curr_point != pt)
                known_distance[pt] = min(known_distance[pt], get_distance(points[curr_point], points[pt]));
    }
    
    printf("%f", ans);

    return 0;
}