#include <bits/stdc++.h>
using namespace std;
//#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    ifstream in;
    ofstream out;
    map<int, bool> isTerminal;
    string word;
    vector<map<char,int>> automata;

    in.open("problem1.in", ios::in);
    int stateN, transN, termN;
    in >> word;
    in >> stateN >> transN >> termN;
    automata.resize(stateN+1);

    for (int i = 0; i < termN; i++) 
    {
        int term;
        in >> term;
        isTerminal.emplace(term, true);
    }
    for (int i = 0; i < transN; i++) 
    {
        int start, end;
        char trans;
        in >> start >> end >> trans;
        automata[start].emplace(trans, end);
    }
    in.close();

    bool accepts;
    int state = 1; 
    for (int i = 0; i < word.length(); i++) {
        auto hasTrans = automata[state].find(word[i]);
        if (hasTrans == automata[state].end()) {
            out.open("problem1.out");
            out << "Rejects";
            out.close();
            return 0;
        }
        state = hasTrans->second;
    }
    accepts = isTerminal.find(state) != isTerminal.end();

    out.open("problem1.out");
    out << (accepts ? "Accepts" : "Rejects");
    out.close();

    return 0;
}