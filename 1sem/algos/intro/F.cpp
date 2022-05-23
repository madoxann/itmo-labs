#include <bits/stdc++.h>
using namespace std;
//#define int long long

void siftUp(vector<int> &arr, int start)
{
    while (arr[start] > arr[(start-1) / 2])
    {
        swap(arr[start], arr[(start-1)/2]);
        start = (start - 1)/2;
    }
}

void add(vector<int> &arr, int val)
{
    arr.push_back(val);   
    if (arr.size() == 1)
    {
        return;
    }
    siftUp(arr, arr.size() - 1);
}

int popMin(vector<int> &arr)
{
    int tmp = arr[0];
    arr.erase(arr.begin());
    for (int i = 0; i < arr.size(); i++)
        siftUp(arr, i);
    return tmp;
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n;
    cin >> n;
    vector<int> heap;

    for (int i = 0; i < n; i++)
    {
        int line, action;
        cin >> action;
        if (!action)
        {
            cin >> line;
            add(heap, line);
        }
        else 
        {
            cout << popMin(heap) << endl;
        }
    }
    return 0;
}