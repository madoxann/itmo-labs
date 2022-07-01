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
    vector<map<char, vector<int>>> automata;

    in.open("problem2.in", ios::in);
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
        if (automata[start].find(trans) == automata[start].end())
            automata[start].emplace(trans, vector(1, end));
        else automata[start][trans].push_back(end);
    }
    in.close();

    bool accepts;
    set<int> states;
    states.emplace(1);
    for (int i = 0; i < word.length(); i++) 
    {
        set<int> newStates;
        for (int state : states)
        {
            // cout << state << endl;
            auto hasTrans = automata[state].find(word[i]);
            // if (hasTrans == automata[state].end()) 
            // {
                // cout.open("problem1.out");
                // cout << "Rejects";
                // cout.close();
                // return 0;
            // }
            if (hasTrans != automata[state].end()) 
                for (int possibleState : hasTrans->second)
                    newStates.emplace(possibleState);
        }
        states = newStates;
    }
    
    out.open("problem2.out");
    for (int endState : states)
    {
        if (isTerminal.find(endState) != isTerminal.end())
        { 
            out << "Accepts";
            out.close();
            return 0;
        }
    }
    out << "Rejects";
    out.close();

    return 0;
}
