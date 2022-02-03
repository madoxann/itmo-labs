#include <bits/stdc++.h>
using namespace std;
//#define int long long

vector<vector<int>> *dp;
int n, m, mod = pow(10,6) + 7;

int go(int y, int x)
{
    if (x < 0 || y < 0 || x > m-1 || y > n-1){
        return 0;
    }

    if ((*dp)[y][x] != -1)
        return (*dp)[y][x];

    (*dp)[y][x] = (go(y-2, x-1) + go(y-2, x+1) + go(y-1, x-2) + go(y+1, x-2)) % mod;
    return (*dp)[y][x];
}


int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    ifstream in;
    ofstream out;

    in.open("knight.in", ios::in);
    //cin >> n >> m;
    in >> n >> m;

    dp = new vector<vector<int>> (n, vector<int>(m, -1));
    (*dp)[0][0] = 1;
    go(n-1, m-1);
    
    out.open("knight.out");
    //cout << (*dp)[n-1][m-1] << endl;
    out << (*dp)[n-1][m-1] << endl;

    return 0;
}