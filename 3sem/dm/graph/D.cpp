#include <bits/stdc++.h>
using namespace std;
//#define int long long

typedef vector<unordered_set<int>> tournament;
typedef unordered_set<int> vertex;

tournament graph;
vector<int> visited;
deque<int> sorted; 

void dfs(vertex parent, int ind) {
    visited[ind] ^= 1;

    for (int n : parent)
        if (!visited[n]) {
            dfs(graph[n], n);
        }
    
    sorted.push_front(ind);
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    
    int n;
    cin >> n;


    graph = vector(n, unordered_set<int>());
    for (int i = 1; i < n; i++)
    {
        string input;
        cin >> input;

        for (int j = 0; j < input.size(); j++)
        {
            if (input[j] == '1') graph[i].emplace(j);
            else graph[j].emplace(i);
        }
    }

    for (int i = 0; i < n; i++)
    {
        visited = vector<int>(n, 0);
        sorted = deque<int>();
        dfs(graph[i], i);

        if (graph[sorted.back()].find(sorted.front()) != graph[sorted.back()].end())
        {
            for (int i : sorted) cout << ++i << " ";
            return 0;
        }
    }

    return 0;
}