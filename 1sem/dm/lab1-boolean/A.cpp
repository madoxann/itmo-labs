#include <bits/stdc++.h>
using namespace std;
int isReflex(vector<vector<int> > arr, int n)
{
    int counter = 0;
    for(vector<int> i : arr)
    {
        if(i[counter])
            counter++;
        else
            return 0;
    }
    return 1;
}
int isAntiReflex(vector<vector<int> > arr, int n)
{
    int counter = 0;
    for(vector<int> i : arr)
    {
        if(!i[counter])
            counter++;
        else
            return 0;
    }
    return 1;
}

int isSymmetrical(vector<vector<int>> arr, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if (arr[i][j] != arr[j][i])
                return 0;
        }
    }
    return 1;
}
int isAntiSymmetrical(vector<vector<int> > arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (arr[i][j] && arr[j][i] && (j != i))
                return 0;
        }
    }
    return 1;
}
int isTransitive(vector<vector<int> > arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if(arr[i][j])
                for (int c = 0; c < n; c++)
                {
                    if (arr[j][c] && !arr[i][c])
                        return 0;
                }
        }
    }
    return 1;
}
int main(int argc, char const *argv[])
{
    int n, currNum;
    vector<vector<int> > arr = {};
    cin >> n;
    for(int i = 0; i < n; i++)
    {
        arr.push_back({});
        for(int j = 0; j < n; j++)
        {
            cin >> currNum;
            arr[i].push_back(currNum);
        }
    }

    vector<vector<int> > arr1 = {};
    for(int i = 0; i < n; i++)
    {
        arr1.push_back({});
        for (int j = 0; j < n; j++)
        {
            cin >> currNum;
            arr1[i].push_back(currNum);
        }
    }

    cout << isReflex(arr, n) << " " << isAntiReflex(arr, n) << " " << isSymmetrical(arr, n) << " " << isAntiSymmetrical(arr, n) << " " << isTransitive(arr, n) << endl;
    cout << isReflex(arr1, n) << " " << isAntiReflex(arr1, n) << " " << isSymmetrical(arr1, n) << " " << isAntiSymmetrical(arr1, n) << " " << isTransitive(arr1, n) << endl;
    for(int i = 0; i < n; i++)
    {
        vector<int> compLine = {};
        for (int tmp = 0; tmp < n; tmp++)
            compLine.push_back(0);
        for(int j = 0; j < n; j++)
        {
            if (arr[i][j])
            {
                for (int c = 0; c < n; c++)
                {
                    if (arr1[j][c])
                    {
                        compLine[c] = 1;
                    }
                }
            }
        }
        for (int tmp = 0; tmp < n; tmp++)
            cout << compLine[tmp] << " ";
        cout << endl;
    }

    return 0;
}
