#include <bits/stdc++.h>
using namespace std;
int main(int argc, char const *argv[])
{
    int n, value;
    cin >> n;
    string truthTable;
 
    vector<int> nullVec = {};    
    vector<vector<int>> operations(n, nullVec);
 
    vector<int> toDisj = {};
    vector<int> notVars(n,0);
    for(int i = 0; i < pow(2, n); i++)
    {
        cin >> truthTable;
        cin >> value;
        if(value)
        { 
            bool firstConj = true;
            //adding not vars
            for(int j = 0; j < n; j++)
            {
                if(!(truthTable[j] - 48))
                {
                    if(!notVars[j])
                    {
                    operations.push_back({1, j+1});
                    //cout << "1 " << j + 1 << endl;
                    notVars[j] = operations.size();
                    //cout << "NOT VARS " << notVars[j] << endl;
                    //lastOper++;
                    }
                }
            }
            //conj logic
            for(int j = 0; j < n - 1; j++)
            {
                vector<int> buildOper(1,2);    
                if(firstConj)
                {
                    if(truthTable[j] - 48)
                        buildOper.push_back(j + 1);
                    else
                        buildOper.push_back(notVars[j]);
                    if(truthTable[j + 1] - 48)
                        buildOper.push_back(j + 2);
                    else
                        buildOper.push_back(notVars[j + 1]);
                    operations.push_back(buildOper);
                    if(n <= 2)
                        toDisj.push_back(operations.size());
                    firstConj = false;
                }
                else
                {
                    if(truthTable[j + 1] - 48)
                        buildOper.push_back(j + 2);
                    else
                        buildOper.push_back(notVars[j + 1]);
                    buildOper.push_back(operations.size());
                    operations.push_back(buildOper);
                    if(j == n - 2)
                        toDisj.push_back(operations.size());
                }
            }
        }
    }
 
    if(operations.size() == n)
    {
        cout << n + 2 << endl;
        cout << "1 1" << endl;
        cout << "2 1 " << n+1 << endl;
        return 0;
    }
 
    bool firstDisj = true;
    for(int i = 0; i < toDisj.size() - 1; i++)
    {       
        vector<int> buildOper(1,3);
        //cout << "3 ";
        if(firstDisj)
        {   
            buildOper.push_back(toDisj[i]);
            buildOper.push_back(toDisj[i+1]);
            //cout << toConj[i] << " " << toConj[i+1] << endl;
            //lastOper++;
            if(buildOper.size())
                operations.push_back(buildOper);
            firstDisj = false;
        }
        else
        {
            buildOper.push_back(operations.size());
            buildOper.push_back(toDisj[i+1]);
            if(buildOper.size())
               operations.push_back(buildOper);
            //cout << lastOper++ << " " << toConj[i+1] << endl; 
        }
    }
 
    cout << operations.size() << endl;
    for (vector<int> operation : operations)
    {
        if(operation.size())
        {
            for(int sign : operation)
            {
                cout << sign << " ";
            }
            cout << endl;
        }
    }
    /*
    //int lastOper = n;
    if(sdnf.size() == 0)
    {
        cout << n+2 << endl;
        cout << "1 1" << endl;
        cout << "2 1 " << n+1 << endl;
        return 0;
    }
    vector<int> nullVec = {};    
    vector<vector<int>> operations(n, nullVec);
     
    vector<int> notVars(n, 0);
    vector<int> toConj = {};
    for(int i = 0; i < sdnf.size(); i++)
    {
        bool firstDisj = true;
        //invert vars
        for(int j = 0; j < sdnf[i].size(); j++)
        {
            if(!sdnf[i][j])
            {
                if(!notVars[j])
                {
                    operations.push_back({1, j+1});
                    //cout << "1 " << j + 1 << endl;
                    notVars[j] = operations.size();
                    //lastOper++;
                }
            }
        }
        //conj var
        vector<int> buildOper = {};
        for(int j = 0; j < sdnf[i].size() - 1; j++)
        {    
            buildOper.push_back(2);
            //cout << "2 ";
            if(firstDisj){    
                if(sdnf[i][j])
                {
                    buildOper.push_back(j+1);
                    //cout << j + 1;
                }
                else
                {
                    buildOper.push_back(notVars[j]);
                    //cout << notVars[j];
                }
                if(sdnf[i][j+1])
                {
                    buildOper.push_back(j+2);
                    //cout << " " << j + 2 << endl;
                }
                else
                {
                    buildOper.push_back(notVars[j+1]);
                    //cout << " " << notVars[j+1] << endl;
                }
                firstDisj = false;
                if(buildOper.size())
                    operations.push_back(buildOper);
                buildOper = {};
                //lastOper++;
            }
            else
            {
                buildOper.push_back(operations.size());
                //cout << lastOper++;
                if(sdnf[i][j+1])
                {
                    buildOper.push_back(j+2);
                    //cout << " " << j + 2 << endl;
                    //lastOper = max(lastOper, j+2);
                }
                else
                {
                    buildOper.push_back(notVars[j+1]);
                    //cout << " " << notVars[j+1] << endl;
                    //lastOper = max(lastOper, notVars[j+1]);
                }
            }
        }
        if(buildOper.size())
            operations.push_back(buildOper);
        toConj.push_back(operations.size());
    }
    bool firstConj = true;
    for(int i = 0; i < toConj.size() - 1; i++)
    {       
        vector<int> buildOper = {};
        buildOper.push_back(3);
        //cout << "3 ";
        if(firstConj)
        {   
            buildOper.push_back(toConj[i]);
            buildOper.push_back(toConj[i+1]);
            //cout << toConj[i] << " " << toConj[i+1] << endl;
            //lastOper++;
            if(buildOper.size())
                operations.push_back(buildOper);
            firstConj = false;
        }
        else
        {
            buildOper.push_back(operations.size());
            buildOper.push_back(toConj[i+1]);
            if(buildOper.size())
               operations.push_back(buildOper);
            //cout << lastOper++ << " " << toConj[i+1] << endl; 
        }
    }
    cout << operations.size() << endl;
    for(int i = n; i < operations.size(); i++)
    {
        for(int op : operations[i])
        {
            cout << op << " ";
        }
        cout << endl;
    }
    */
    return 0;
}