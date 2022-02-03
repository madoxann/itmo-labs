#include <bits/stdc++.h>
using namespace std;
 
//int answer = 0;
#define int long long
vector<int> merge(vector<int> arrL, vector<int> arrR, int& answer) 
{
    vector<int> mergedArr;
    int iterL = 0, iterR = 0; 
    while (iterL != arrL.size() && iterR != arrR.size())
    {
        if (arrL[iterL] > arrR[iterR]) 
        {
            mergedArr.push_back(arrR[iterR++]);
            answer += arrL.size() - iterL;
        }
        else
        {
            mergedArr.push_back(arrL[iterL++]);
        }
    }
    if (iterL != arrL.size()) 
    {
        for (int i = iterL; i < arrL.size(); i++) mergedArr.push_back(arrL[i]);
    }
    else
    {
        for (int i = iterR; i < arrR.size(); i++) mergedArr.push_back(arrR[i]);
    }
    return mergedArr;
}
 
vector<int> mergeSort(vector<int> arr, int& answer) 
{
    if (arr.size() == 1) return arr;
    vector<int> leftHalf(arr.size()/2, 0);
    vector<int> rightHalf(arr.size() - arr.size() /2, 0);
    for (int i = 0; i < arr.size()/2; i++) 
    {
        leftHalf[i] = arr[i];
    }
    for(int i = arr.size()/2; i < arr.size(); i++)
    {
        rightHalf[i - arr.size()/2] = arr[i];
    }
    leftHalf = mergeSort(leftHalf, answer);
    rightHalf = mergeSort(rightHalf, answer);
    return merge(leftHalf, rightHalf, answer);
}
 
int32_t main()
{
    int n, counter = 0;
    cin >> n;
    vector<int> arr(n, 0);
    for (int i = 0; i < n; i++) 
    {
        cin >> arr[i];
    }
    int answer = 0;
    mergeSort(arr, answer);
    cout << answer << endl;
    return 0;
}