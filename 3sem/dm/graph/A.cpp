#include <bits/stdc++.h>
using namespace std;
//#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int n;
    cin >> n;

    vector<vector<int>> graph = vector(n, vector(n, 0));
    for (int i = 1; i < n; i++)
    {
        string input;
        cin >> input;

        for (int j = 0; j < input.size(); j++)
        {
            if (input[j] == '1')
                graph[i][j] = graph[j][i] = 1;
        }
    }

    deque<int> deq;
    for (int i = 0; i < n; i++)
        deq.push_back(i);

    for (int i = 0; i < n * (n - 1); i++)
    {
        if (!graph[deq[0]][deq[1]])
        {
            auto j = deq.begin() + 2;
            for (; j != deq.end(); j++)
                if (graph[deq[0]][*j] && graph[deq[1]][*(j + 1)])
                    break;

            reverse(deq.begin() + 1, j + 1);
        }
        deq.push_back(deq[0]);
        deq.pop_front();
    }

    for (auto i : deq)
        cout << ++i << " ";

    return 0;
}