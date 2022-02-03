def binSearchUp(array, val): 
    l = -1
    r = len(array)
    while (r - l > 1):
        mid = (l + r)//2
        if array[mid] <= val:
            l = mid
        else:
            r = mid
    return r

def binSearchDown(array, val): 
    l = -1
    r = len(array)
    while (r - l > 1):
        mid = (l + r)//2
        if array[mid] >= val:
            r = mid
        else:
            l = mid
    return r


n = int(input())
arr = list(map(int, input().split()))
arr.sort()
ans = ""
k = int(input())
for i in range(k):
    l, r = map(int, input().split())
    lBin = binSearchDown(arr, l)
    rBin = binSearchUp(arr, r)
    ans += str(abs(lBin - rBin)) + " "
    """f l == arr[lBin]:
        lBin = getFarmost(arr, lBin)
    if r == arr[rBin]:
        rBin = getFarmost(arr, rBin)
    if arr[lBin] < l or rBin == lBin:
        ans += str(rBin - lBin) + " "
    else:
        ans += str(rBin - lBin + 1) + " "
    """

print(ans)