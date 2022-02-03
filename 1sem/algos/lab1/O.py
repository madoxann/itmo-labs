import math
def func(x):
    return x**2 + math.sqrt(x)

def evaluate(x):
    l = 0
    r = math.sqrt(x) + 1
    percision = 0.00000001
    while abs(l-r) > percision:
        mid = (l + r) / 2
        result = func(mid)
        if abs(result - x) <= percision:
            return mid
        if result < x:
            l = mid
        else:
            r = mid
    return r        

c = float(input())
print(evaluate(c))
