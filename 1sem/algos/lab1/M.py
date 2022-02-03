def binSearch(array, val): 
    l = -1
    r = len(array)
    while (r - l > 1):
        mid = (l + r)//2
        if array[mid] == val:
            return mid
        if array[mid] < val:
            l = mid
        else:
            r = mid
    if l == -1:
        return 0
    elif r == n:
        return n - 1
    if abs(val - array[r]) < abs(val - array[l]):
        return r
    return l


n, k = map(int, input().split())
arr = list(map(int, input().split()))
query = list(map(int, input().split()))

for i in query:
    print(arr[binSearch(arr, i)])