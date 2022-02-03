#include <bits/stdc++.h>
using namespace std;
//#define int long long

string alph = "abcdefghijklmnopqrstuvwxyz";

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    string inArr;
    cin >> inArr;
    for (char item : inArr)
    {
        for (int i = 0; i < alph.size(); i++)
        {
            if (alph[i] == item)
            {
                cout << i + 1 << " "; 
                char temp = alph[i];
                alph.erase(i,1);
                alph = temp + alph;
                //cout << alph;
            }
        }
    }
    cout << endl;

    return 0;
}