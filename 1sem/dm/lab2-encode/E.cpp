#include <bits/stdc++.h>
using namespace std;
//#define int long long
 
string alph = "abcdefghijklmnopqrstuvwxyz";
string buffer = "";
vector<string> vocab;
 
int isInVocab(string str)
{
    for (int i = 0; i < vocab.size(); i++)
    {
        if (str == vocab[i]) return i;
    }
    return -1;
}
 
int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    string arrIn;
    for (char item : alph)
    {
        vocab.push_back(string(1,item));
    }
 
    cin >> arrIn;
    for (char item : arrIn)
    {
        string found = buffer + item;
        int vocabPos = isInVocab(found);
        if (vocabPos != -1) 
        {
            buffer = found;
        }  
        else
        {
            cout << isInVocab(buffer) << " ";
            vocab.push_back(found);
            buffer = item;
        }
    }
     
    cout << isInVocab(buffer) << endl;
     
    return 0;
}