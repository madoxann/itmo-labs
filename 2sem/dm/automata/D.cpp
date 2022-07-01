#include <bits/stdc++.h>
using namespace std;
#define int long long
  
vector<vector<int>> matrix;

struct Matrix
{
    vector<vector<int>> vals;
    int dimH;
    int dimW;

    Matrix(vector<vector<int>> v) 
    {
        vals = v;
        dimH = v.size();
        dimW = v[0].size();
    }
    void print(Matrix m)
    {
        for (int i = 0; i < m.dimH; i++)
        {
            for (int j = 0; j < m.dimW; j++)
            {
                cout << m.vals[i][j] << " ";
            }
            cout << endl;
        }
    }
};

Matrix operator*(const Matrix& l, const Matrix& r) 
{
    Matrix* ret = new Matrix(vector(l.dimH, vector<int>(r.dimW, 0)));
    for (int c = 0; c < l.dimW; c++)
    {
        for (int i = 0; i < ret->dimH; i++) 
        { 
            for (int j = 0; j < ret->dimW; j++)
            {
                ret->vals[i][j] = (ret->vals[i][j] + l.vals[i][c] * r.vals[c][j]) % 1000000007;
            }
        }
    }
    return *ret;  
}

#define pMatrix Matrix*

Matrix pow(Matrix m, int power)
{
    if (power == 1) return m;
    if (power % 2 == 0)
    {
        Matrix m1 = pow(m, power/2);
        return m1 * m1;
    }
    return m * pow(m, power-1);
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    ifstream in;
    ofstream out;
    vector<int> terminal;
    vector<map<char,int>> automata;
    vector<vector<int>> graphRev;
 
    in.open("problem4.in", ios::in);
    int stateN, transN, termN, moves;
    in >> stateN >> transN >> termN >> moves;
    Matrix matrix(vector(stateN, vector(stateN, (int) 0)));

    for (int i = 0; i < termN; i++) 
    {
        int term;
        in >> term;
        // may be too slow
        terminal.push_back(term);
    }
    for (int i = 0; i < transN; i++) 
    {
        int start, end;
        char trans;
        in >> start >> end >> trans;
        matrix.vals[--start][--end]++;
    }
    matrix = pow(matrix, moves);

    out.open("problem4.out");
    int ans = 0;
    for (int i : terminal) ans = (ans + matrix.vals[0][i-1]) % 1000000007;
    out << ans;
    out.close();
 
    return 0;
}
