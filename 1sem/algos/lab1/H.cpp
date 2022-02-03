#include <bits/stdc++.h>
using namespace std;
//#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    string opening = "([{";
    string closing = ")]}";

    string brSeq = "";
    cin >> brSeq;

    stack<char> brakets;
    for (int i = 0; i < brSeq.length(); i++)
    {
        if (opening.find(brSeq[i]) != string::npos)
        {
            brakets.push(brSeq[i]);
        }
        else
        {
            if (!brakets.empty() && opening.find(brakets.top()) == closing.find(brSeq[i]))
            {
                brakets.pop();
            }
            else
            {
                cout << "NO" <<  endl;
                return 0;
            }
        }
    }
    if (brakets.empty())
    {
        cout << "YES" << endl;
    }
    else
    {
        cout << "NO" << endl;
    }


    return 0;
}