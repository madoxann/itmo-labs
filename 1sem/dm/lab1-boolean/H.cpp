#include <bits/stdc++.h>
using namespace std;
int main(int argc, char const *argv[])
{
    int index;
    string baseStr = "((A0|B0)|(A0|B0))";
    cin >> index;
    for(int i = 1; i < index; i++)
    {
        string strIndex = to_string(i);
        baseStr = "((" + baseStr + "|((A" + strIndex + "|A" + strIndex + ")|(B" + strIndex + "|B" + strIndex + ")))|(A" + strIndex + "|B" + strIndex +"))";
    }
    cout << baseStr;
 
    return 0;
}