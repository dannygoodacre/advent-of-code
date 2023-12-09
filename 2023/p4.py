data = [line[line.find(':') + 2:].split('|') 
        for line in open("input.txt").read().splitlines()]
scores = []
counts = [1] * len(data)

for i, pair in enumerate(data):
    left = [n for n in pair[0].split()]
    right = [n for n in pair[1].split()]
    score = len(set(left) & set(right))
    scores.append(score)

    for j in range(score):
        counts[i + j + 1] += counts[i]

print(f"1: {sum([int(2**(s - 1)) for s in scores])}\n2: {sum(counts)}")