#include <bits/stdc++.h>
using namespace std;
 
int n;
bool has1 = true, has0 = true, hasSelfDual = true, hasMonotone = true, hasLinear = true;
 
void is1(string func)
{
    if(has1){
        has1 = (func[func.size() - 1] == '1');
    }
    return;
}
 
void is0(string func)
{
    if(has0){
        has0 = (func[0] == '0');
    }
    return;
}
 
void isSelfDual(string func)
{
    if(func.size() == 1){
        hasSelfDual = false;
        return;
    }
    if(hasSelfDual){
        for(int i = 0; i < func.size()/2; i++)
        {
            if(func[i] == func[func.size() - i - 1])
            {
                hasSelfDual = false;
                return;
            }
        }   
    }
    return;
}
 
bool isMonotone(string func)
{
    if(func.size() == 1)
        return true;
    for(int i = func.size(); i > 1 ; i/=2)
    {
        for(int j = 0; j < i / 2; j++)
        {
            if(func[j] > func[i/2 + j])
                return false;
        }
    }
    return isMonotone(func.substr(func.size()/2,func.size())) && isMonotone(func.substr(0, func.size()/2));
}
 
void isLinear(string func)
{
    if(hasLinear){
        if(func.size() == 1)
        {
            return;
        }
        vector<int> currCoff = {};
        int range = func.size();
        for(int i = range; i > 0; i--)
        {
            for(int j = 0; j < i - 1; j++)
            {
                currCoff.push_back((int)func[j] ^ func[j+1]);
            }
            if(currCoff[0])
            {
                if(bitset<32>(range - i + 1).count() > 1)
                {
                    hasLinear = false;
                    break;
                }
            }
            func = "";
            for(int j = 0; j < currCoff.size(); j++)
            {
                func += to_string(currCoff[j]);
            }
            currCoff = {};
        }
    }
    return;
}
 
int main(int argc, char const *argv[])
{
    int currN;
    cin >> n;
    vector<string> strInVec = {};
    for(int i = 0; i < n; i++)
    {
        string strInput;
        cin >> currN >> strInput;
        strInVec.push_back(strInput);
    } 
    for(string strIn : strInVec)
    {
        is0(strIn);
        is1(strIn);
        isSelfDual(strIn);
        if (hasMonotone)
        {
            hasMonotone = isMonotone(strIn);
        }
        isLinear(strIn);
    }
    //cout << has0 << has1 << hasLinear << hasMonotone << hasSelfDual << endl;
    if(has0 || has1 || hasLinear || hasMonotone || hasSelfDual)
    {
        cout << "NO";
    }
    else
    {
        cout << "YES";
    }
    return 0;
}