#include <bits/stdc++.h>
using namespace std;
//#define int long long

unordered_map<char, unordered_set<string>> grammar;

bool hasStr(string str, char currState, int currPos)
{
    if (currPos > str.size()-1) return false;
    for (auto trans : grammar[currState])
    {
        if (str[currPos] == trans[0]) 
        {
            if (trans.size() == 1 && str.size()-1 == currPos || 
                hasStr(str, trans[1], currPos+1)) return true;
        }
    }
    return false;
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    ifstream in;
    ofstream out;    

    in.open("automaton.in", ios::in);
    int stateN;
    char start; 
    in >> stateN >> start;

    for (int i = 0; i < stateN; i++)
    {
        unordered_set<string> insert;
        string tmp, trans;
        char Nterminal;
        in >> Nterminal >> tmp >> trans;
        insert.emplace(trans);
        if (auto hasTrans = grammar.find(Nterminal); hasTrans != grammar.end()) 
            hasTrans->second.emplace(trans);
        else
            grammar.emplace(Nterminal, insert);
    }

    int n;
    in >> n;

    out.open("automaton.out");
    for (int i = 0; i < n; i++)
    {
        string str;
        in >> str;

        out << (hasStr(str, start, 0) ? "yes" : "no") << "\n";
    }
    in.close();
    out.close();

    return 0;
}
