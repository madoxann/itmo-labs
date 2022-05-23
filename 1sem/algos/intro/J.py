n = int(input())
dirTree = []
for i in range(n):
    dirTree.append(input())

dirTree.sort()
home = dirTree[0].count('/')
recursiveLvl = home
cnt = 0
for dir in dirTree:
    dir = dir.split('/')
    print("  " * (len(dir) - 1) + dir[-1])