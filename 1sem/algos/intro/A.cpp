#include <bits/stdc++.h>
using namespace std;

#define int long long

int n;
vector<int> arr;

void swap(int l, int r)
{
    int temp = arr[l];
    arr[l] = arr[r];
    arr[r] = temp;
}

int partition(int l, int r)
{
    int base = arr[(l + r)/2];
    int i = l;
    int j = r;
    while (i <= j)
    {
        while (arr[i] < base) i++;
        while (arr[j] > base) j--;
        if (i >= j) break;
        swap(i++, j--);
    }
    return j;
}


void quicksort(int l, int r)
{
    if (l < r)
    {
        int parted = partition(l, r);
        quicksort(l, parted);
        quicksort(parted + 1, r);
    }
}


int32_t main()
{
    cin >> n;
    arr.resize(n);
    for (int i = 0; i < n ; i++)
    {
        cin >> arr[i];
    }
    quicksort(0, n-1);
    for(int i : arr) cout << i << " ";
    return 0;
}