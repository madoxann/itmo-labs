#include <bits/stdc++.h>
using namespace std;
//#define int long long
 
string nextBBS(string seq)
{
    int openCnt = 0, closeCnt = 0, i = seq.size()-1;
    for (i; i > -1; i--)
    {
        if (seq[i] == '(') 
        {
            openCnt++;
            if (openCnt < closeCnt && openCnt != 0) break;
        }
        if (seq[i] == ')') closeCnt++;
    }
    if (i > 0)
    {
        //cout << "SUBSTR " << seq.substr(0,i) << endl; 
        return seq.substr(0,i) + ')' + string(openCnt, '(') + string(closeCnt-1,')');
    }
    return "-";
}
 
int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
 
    string seq;
    cin >> seq;
 
    seq = nextBBS(seq);
    cout << seq << endl;
    return 0;
}
