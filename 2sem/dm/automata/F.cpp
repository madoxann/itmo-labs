#include <bits/stdc++.h>
using namespace std;
// #define int long long

typedef vector<map<char,int>> automata;

vector<bool> visited;
automata automata1;
automata automata2;
set<int> term1;
set<int> term2;
map<int, int> isomorph;

bool findDevil(automata aut, int state)
{
    for (auto trans : aut[state])
        if (trans.second != state) 
            return false;
    return true;
}

bool dfs(int state1, int state2)
{
    visited[state1] = true;
    if ((term1.find(state1) == term1.end()) != 
    (term2.find(state2) == term2.end())) return false;

    bool concat = true;
    isomorph.emplace(state1, state2);
    for (auto trans : automata1[state1])
    {
        state1 = automata1[state1][trans.first];
        state2 = automata2[state2][trans.first];
        if (findDevil(automata1, state1) !=
            findDevil(automata2, state2)) return false;
        if (visited[state1]) concat &= (isomorph[state1] == state2);
        else concat &= dfs(state1, state2);
    }
    return concat;
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    ifstream in;
    ofstream out;

    in.open("isomorphism.in", ios::in);
    int stateN, transN, termN;
    in >> stateN >> transN >>termN;
    automata1.resize(stateN+1);
    automata2.resize(stateN+1);
    visited.resize(stateN+1);

    for (int i = 0; i < termN; i++) 
    {
        int terminal;
        in >> terminal;
        term1.emplace(terminal);
    }
    for (int i = 0; i < transN; i++) 
    {
        int start, end;
        char trans;
        in >> start >> end >> trans;
        automata1[start].emplace(trans, end);
    }
    int stateN2, transN2, termN2;
    in >> stateN2 >> transN2 >> termN2;
    if (stateN != stateN2 || transN != transN2 || termN != termN2) 
    {
        in.close();
        out.open("isomorphism.out");
        out << "NO";
        out.close();
        return 0;
    }

    for (int i = 0; i < termN; i++) 
    {
        int terminal;
        in >> terminal;
        term2.emplace(terminal);
    }
    for (int i = 0; i < transN; i++) 
    {
        int start, end;
        char trans;
        in >> start >> end >> trans;
        automata2[start].emplace(trans, end);
    }
    in.close();

    out.open("isomorphism.out");
    out << (dfs(1,1) ? "YES" : "NO");
    out.close();
 
    return 0;
}