#include <bits/stdc++.h>
using namespace std;
#define int long long

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
    void print()
    {
        for (int i = 0; i < this->dimH; i++)
        {
            for (int j = 0; j < this->dimW; j++)
            {
                cout << this->vals[i][j] << " ";
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

vector<map<char,int>> thompson(vector<map<char, vector<int>>> nfa, set<char> sigma, set<int>& terminal, int& stateN)
{
    vector<map<char, int>> dfa;
    queue<set<int>> states;
    set<int> newTerminal;
    set<set<int>> usedStates;
    map<set<int>, int> mapToNums;

    int nextState = 0;
    states.push({1});
    while (!states.empty())
    {
        set<int> current = states.front();
        states.pop();
        for (char letter : sigma)
        {
            set<int> newState;
            bool hasTerminal = false;
            for (int item : current)
            {
                if (auto hasTrans = nfa[item].find(letter); hasTrans != nfa[item].end()) 
                {
                    for (int possibleState : hasTrans->second)
                    {
                        newState.emplace(possibleState);
                        if (auto hasTerm = terminal.find(possibleState); hasTerm != terminal.end())
                            hasTerminal = true;
                    }
                }          
            }    
            if (newState.size() > 0)
            {
                // mapping vector to numbers for matrix
                if (auto hasVec = mapToNums.find(current); hasVec == mapToNums.end())
                    mapToNums.emplace(current, nextState++);
                if (auto hasVec = mapToNums.find(newState); hasVec == mapToNums.end())
                    mapToNums.emplace(newState, nextState++);
                int currInd = mapToNums[current];
                if (dfa.size() < currInd+1) dfa.resize(currInd+2);
                dfa[currInd].emplace(letter, mapToNums[newState]);
                if (hasTerminal) newTerminal.emplace(mapToNums[newState]);
                if (usedStates.find(newState) == usedStates.end())
                {
                    usedStates.emplace(newState);
                    states.push(newState);
                }
            }
        }
    }
    stateN = nextState;
    terminal = newTerminal;
    return dfa;
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    ifstream in;
    ofstream out;

    string word;
    set<int> terminal;
    set<char> sigma;
    vector<map<char, vector<int>>> automata;

    in.open("problem5.in", ios::in);

    int stateN, transN, termN, moves;
    in >> stateN >> transN >> termN >> moves;
    automata.resize(stateN+1);

    for (int i = 0; i < termN; i++) 
    {
        int term;
        in >> term;
        terminal.emplace(term);
    }
    for (int i = 0; i < transN; i++) 
    {
        int start, end;
        char trans;
        in >> start >> end >> trans;
        sigma.emplace(trans);
        if (automata[start].find(trans) == automata[start].end())
            automata[start].emplace(trans, vector(1, end));
        else automata[start][trans].push_back(end);
    }

    int len = 0;
    vector<map<char, int>> dfa = thompson(automata, sigma, terminal, len);
    Matrix matrix(vector(len, vector(len, (int) 0)));

    for (int i = 0; i < len; i++)
    {
        for (char letter : sigma)
        {
            if (auto hasTrans = dfa[i].find(letter); hasTrans != dfa[i].end())
            {
                matrix.vals[i][hasTrans->second]++;
            }
        }
    }

    matrix.print();
    cout << endl;
    matrix = pow(matrix, moves);
    matrix.print();
    cout << endl;

    out.open("problem5.out");
    int ans = 0;
    for (int i : terminal) ans = (ans + matrix.vals[0][i]) % 1000000007;
    out << ans;
    out.close();

    return 0;
}
