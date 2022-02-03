n = int(input())

arr = list(map(int, input().split()))
ans = 0
while(len(arr) > 1):
    min1 = min(arr)
    indexMin1 = arr.index(min1)
    min2 = min(arr[:indexMin1] + arr[indexMin1+1:])
    arr.append(min1 + min2)
    ans += min1 + min2
    arr.pop(indexMin1)
    arr.pop(arr.index(min2))

print(ans)
