#include <bits/stdc++.h>
using namespace std;
//#define int long long

struct stackWithMin
{
    stack<int> val;
    stack<int> currMin;

    void push(int value)
    {
        val.push(value);
        if (currMin.empty())
        {
            currMin.push(value);
        }
        else
        {
            currMin.push(min(value, currMin.top()));
        }
    }

    int pop()
    {
        if (val.empty())
            return -1;
        int tmp = val.top();
        val.pop();
        currMin.pop();
        return tmp;
    }

    bool isEmpty()
    {
        return val.empty();
    }

    int getMin()
    {
        if (currMin.empty())
        {
            return INT_MAX;
        }
        return currMin.top();
    }
};

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    stackWithMin left;
    stackWithMin right;

    int n;
    cin >> n;
    
    for (int i = 0; i < n; i++)
    {
        char sign;
        int num;
        cin >> sign;
        
        if (sign == '+')
        {
            cin >> num;
            right.push(num);
            cout << min(left.getMin(), right.getMin()) << endl;
        }
        
        else
        {
            if (left.isEmpty())
            {
                while (!right.isEmpty())
                {
                    left.push(right.pop());       
                }
            }
            left.pop();
            if (left.isEmpty() && right.isEmpty())
            {
                cout << -1 << endl;
            }
            else
            {
                cout << min(left.getMin(), right.getMin()) << endl;
            }
        }
    }

    return 0;
}