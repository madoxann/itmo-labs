#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char const *argv[])
{
    int n, currVal;
    cin >> n;
    vector<int> values = {};
    string useless = "";
    for (int i = 0; i < pow(2, n); i++) {
        cin >> useless >> currVal;
        values.push_back(currVal);
    }
 
    vector<int> currCoff = {};
    vector<int> ans = {values[0]};
    for(int i = pow(2, n); i > 0; i--)
    {
        for(int j = 0; j < i - 1; j++)
        {
            currCoff.push_back(values[j] ^ values[j+1]);
        }
        ans.push_back(currCoff[0]);
        values = currCoff;
        currCoff = {};
    }
    for(int i = 0; i < pow(2, n); i++)
    {
        cout << (bitset<10>(i).to_string()).substr(10-n,10) << " " <<  ans[i] << endl;
    }
    return 0;
}