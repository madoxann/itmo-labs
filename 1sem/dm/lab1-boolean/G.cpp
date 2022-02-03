#include <bits/stdc++.h>
using namespace std;
int main(int argc, char const *argv[])
{
    long long n, currNum, answer;
    cin >> n;
    vector<bitset<64>> numbers(n, 0);
    for(int i = 0; i < n; i++)
    {
        cin >> currNum;
        numbers[i] = bitset<64>(currNum).to_ulong();
    } 
    cin >> answer;
    bitset<64> answerBin(answer);
    map<string, int> truthTable = {};
    for(int i = 0; i < 64; i++)
    {
        string buildTable = "";
        for(int j = 0; j < n; j++)
            buildTable += numbers[j][i];
        pair<string, int> insertToDict = make_pair(buildTable, answerBin[i]);
        if(truthTable.count(insertToDict.first) == 1)
        {
            if(truthTable.find(insertToDict.first)->second != insertToDict.second)
            {
                cout << "Impossible";  
                return 0;
            }
        }
        truthTable.insert(insertToDict);
    }
 
 
    string actions = "";    
    int amountOfActions = 0;
    for(int i = 0; i < 64; i++)
    {
        if(answerBin[i])
        {
            string buildActions = "(";
            for(int j = 0; j < n; j++)
            {
                if(j > 0)
                    buildActions += "&";
                if(numbers[j][i] == 0)
                    buildActions += "~";
                buildActions += to_string(j+1);
            }
            amountOfActions++;
            buildActions += ")|";
            actions += buildActions;
        }
    }
    if(actions.size() == 0)
    {
        cout << "1&(~1)";
        return 0;
    }
    if(amountOfActions == 1)
        cout << actions.substr(1, actions.size()-3);
    else
        cout << actions.substr(0, actions.size() - 1);
    return 0;
}