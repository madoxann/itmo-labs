    #include <bits/stdc++.h>
    using namespace std;
    #define int long long
     
    int binSearch(vector<int> arr, int val)
    {
        int l = -1, r = arr.size();
        while (r - l > 1)
        {
            int mid = (l + r) / 2;
            if (arr[mid] == val) 
            {
                return mid;
            }
            if (arr[mid] < val)
            {
                l = mid;
            }
            else
            {
                r = mid;
            }
        }
        //cout << l << " " << r;
        if (abs(arr[r] - val) < abs(arr[l] - val))
        {
            return r;
        }
        return l;
    }
     
    int32_t main()
    {
        ios::sync_with_stdio(false);
        cin.tie(0); cout.tie(0);
     
        int n, k;
        cin >> n >> k;
        vector<int> arr(n, 0);
        vector<int> query(k,0);
        map<int, int> prevQuery;
        
        for (int i = 0; i < n; i++)
        {
            cin >> arr[i];
        }
        for (int i = 0; i < k; i++)
        {
            cin >> query[i];
        }
     
        for (int item : query)
        {
            //cout << item << endl;
            if (prevQuery.count(item) > 0)
            {
                //cout << "mem" << endl;
                cout << arr[prevQuery.find(item)->second] << endl;
            }
            else 
            {
                int res = binSearch(arr, item);
                cout << arr[res] << endl; 
                prevQuery.emplace(item, res);
            }
        }
     
        return 0;
    }