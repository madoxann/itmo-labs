#include <bits/stdc++.h>
using namespace std;
//#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); std::cout.tie(0);
    ifstream in;
    ofstream out;

    int n, m;
    in.open("input.txt", ios::in);
    in >> n >> m;
    vector<vector<int>> matrix(n, vector<int>(m, 0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            in >> matrix[i][j];
        }
    }

    vector<vector<int>> matrixAns(n, vector<int>(m, 0));
    matrixAns[0][0] = matrix[0][0];
    for (int i = 1; i < n; i++)
        matrixAns[i][0] = matrixAns[i-1][0] + matrix[i][0];
    for (int j = 1; j < m; j++)
        matrixAns[0][j] = matrixAns[0][j-1] + matrix[0][j];

    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < m; j++)
        {
            matrixAns[i][j] += max(matrixAns[i-1][j], matrixAns[i][j-1]) + matrix[i][j];
        }
    }

    out.open("output.txt");
    out << matrixAns[n-1][m-1] << endl;
    int x = m-1, y = n-1;
    string ans = ""; 
    while (x + y != 0)
    {
        if (matrixAns[y][x] - matrix[y][x] == matrixAns[y][x-1])
        {
            x--;
            ans += "R";
        }
        else 
        {
            y--;
            ans += "D";
        }
    }
    for (int i = ans.size()-1; i > -1; i--)
    {
        out << ans[i];
    }
    out << endl;

    return 0;
}