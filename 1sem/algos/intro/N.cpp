#include <bits/stdc++.h>
using namespace std;
#define int long long

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    //Line autism (should it be faster?)
    int n, x, y;
    cin >> n >> x >> y;
    if (x > y) swap(x, y);
    int time = x, yTime = 0;
    n--;
    while (n > 0)
    {
        if((y - yTime) < x && n == 1) 
        {
            break;
        }
        yTime += x;
        n -= yTime / y;
        yTime = yTime % y;
        time += x;
        n--;
    }
    cout << time << endl;
    return 0;
}