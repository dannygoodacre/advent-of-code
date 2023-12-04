total = 0

for line in open("input.txt").read().splitlines():
    digits = ''.join(c for c in line if c.isdigit())
    total += int(digits[0] + digits[-1])

print("1: " + str(total))

total = 0
rep = {"one" : "1", "two" : "2", "three" : "3", "four" : "4", "five" : "5", 
       "six" : "6", "seven" : "7", "eight" : "8", "nine" : "9"}

for line in open("input.txt").read().splitlines():
    n = len(line)

    res = [line[i:j] for i in range(n) for j in range(i + 1, min(i + 6, n + 1)) 
        if line[i:j] in rep or line[i:j] in rep.values()]
    
    total += int(rep.get(res[0], res[0]) + rep.get(res[-1], res[-1]))

print("2: " + str(total))
