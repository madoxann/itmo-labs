#include <bits/stdc++.h>
using namespace std;
int main(int argc, char const *argv[])
{
    int n, varCount = 0;
    vector<vector<int>> funcVars = {};
    vector<vector<int>> funcVal = {};
    cin >> n;
    for(int i = 0; i < n; i++)
    {
        int currN = 0, currIn;
        cin >> currN;
        if(currN)
        {   
            vector<int> currInput = {};
            for(int j = 0; j < currN; j++)
            {
                cin >> currIn;
                currInput.push_back(currIn);
            }
            funcVars.push_back(currInput);
            currInput = {};
            for(int j = 0; j < pow(2, currN); j++)
            {
                cin >> currIn;
                currInput.push_back(currIn);
            }
            funcVal.push_back(currInput);
        } 
        else
        {
            funcVars.push_back({});
            funcVal.push_back({});
            varCount++;
        }
    }
 
    int depth = 0;
    vector<int> ans = {};
    for(int i = 0; i < pow(2, varCount); i++)
    {
        int variablesUsed = 0, maxDep = 0;
        vector<int> calculated = {};
        vector<int> usedVariables = {};
        string currTableLine = bitset<32>(i).to_string().substr(32-varCount, 32);
        for(int j = 0; j < funcVars.size(); j++)
        {
            string result = "";
            if(funcVars[j].size() > 0)
            {
                //value of function is value of concatenated varialbes in currTable
                for(int value : funcVars[j])
                {
                    usedVariables[value-1]++; 
                    maxDep = max(maxDep, usedVariables[value-1]);
                    result += to_string(calculated[value-1]);
                }
                int givenRes = bitset<32>(result).to_ulong();
                calculated.push_back(funcVal[j][givenRes]);
                usedVariables.push_back(maxDep);
            }
            else
            {
                usedVariables.push_back(0);
                calculated.push_back(currTableLine[variablesUsed++] - 48);
            }
        }
        depth = max(maxDep, depth);
        ans.push_back(calculated[calculated.size()-1]);
    }
    cout << depth << endl;
    for (int value : ans)
        cout << value;
    return 0;
}
