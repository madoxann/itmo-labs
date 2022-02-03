#include <bits/stdc++.h>
using namespace std;
#define int long long

long double func(long double x)
{
    return pow(x, 2) + sqrt(x);
}


long double evaluate(long double c)
{
    long double l = 0;
    long double r = sqrt(c) + 1;
    long double percision = 0.00000001;
    //cout << func(l)<< " " << func(r) << endl; 
    while (abs(l - r) > percision)
    {
        //cout <<func(l) << " " << func(r) << endl;
        long double mid = (l + r) / 2;
        long double result = func(mid);
        //cout << mid << endl;
        //return 0;
        if (abs(result - c) <= percision) 
        {
            return mid;
        } 
        if (result < c)
        {
            l = mid;
        }
        else 
        {
            r = mid;
        }
    }
    return r;
}


int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    /*
    for (long double i = 999999999.8374824787432; i < 1000000000; i+=0.000001)
    {
        long double val = func(i);
        long double ret = evaluate(val);
        if ((ret - i) > 0.0000001)
        {
            cout << "FAILED" << endl;
            cout << i << " GOT " << ret << endl; 
            break;
        } 
        cout << "PASSED" << endl;
    }*/

    long double c;
    cin >> c;
    cout << evaluate(c) << endl;
    

    return 0;
}