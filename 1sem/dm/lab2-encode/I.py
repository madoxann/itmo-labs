import math

n = int(input())

if n == 1:
    string = input()
    it = 0
    encodedLen = 0
    while it < len(string):
        if pow(2, encodedLen) == it + encodedLen + 1:
            encodedLen += 1
        else:
            it += 1
    counter = 0
    counter1 = 0
    ans = ["0" for i in range(0, len(string) + encodedLen)]
    insertPos = []
    for i in range(1, len(string) + encodedLen + 1):
        if pow(2, counter) == i:
            ans[i - 1] = "0"
            counter += 1
            insertPos.append(i - 1)
        else:
            ans[counter + counter1] = string[counter1]
            counter1 += 1
    matrix = [[] for i in range(encodedLen + 1)]
    matrix[0] = ans
    for i in range(1, len(ans) + 1):
        line = bin(i)[2:]
        line = ("0" * (encodedLen - len(line))) + line
        line = line[::-1]
        counter = 0
        for j in range(1, len(matrix)):
            matrix[j].append(line[counter])
            counter += 1
    insertVal = []
    for i in range(1, len(matrix)):
        r = 0
        for j in range(len(ans)):
            r += int(matrix[0][j]) * int(matrix[i][j])
        insertVal.append(r % 2)
    for i in range(len(insertPos)):
        ans[insertPos[i]] = str(insertVal[i])
    print("".join(ans))
else:
    string = input()
    encodedLen = math.ceil(math.log2(len(string) + 1))
    matrix = [[] for i in range(encodedLen + 1)]
    matrix[0] = string
    for i in range(1, len(string) + 1):
        line = bin(i)[2:]
        line = ("0" * (encodedLen - len(line))) + line
        line = line[::-1]
        counter = 0
        for j in range(1, len(matrix)):
            matrix[j].append(line[counter])
            counter += 1

    errorCode = ""
    for i in range(1, len(matrix)):
        s = 0
        for j in range(len(string)):
            s += int(matrix[0][j]) * int(matrix[i][j])
        errorCode += str(s % 2)
    errorCode = int(errorCode[::-1], 2) - 1
    if errorCode > 0:
        string = string[:errorCode] + str((int(string[errorCode]) + 1) % 2) + string[errorCode + 1:]

    counter = 0
    counter1 = 0
    ans = ["0" for i in range(0, len(string) - encodedLen)]
    for i in range(1, len(string) + 1):
        if pow(2, counter) == i:
            counter += 1
        else:
            ans[counter1] = string[counter + counter1]
            counter1 += 1
    print("".join(ans))
