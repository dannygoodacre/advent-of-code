lines = open('input.txt').read().splitlines()

seeds = list(map(int, lines[0].removeprefix('seeds: ').split(' ')))

lines = lines[2:]
lines.append('')

maps = []
current = []

i = 0
doThing = False
for line in lines:
    if line.endswith(':'):
        doThing = True
        continue

    if line == '':
        doThing = False
        maps.append(current)
        current = []
    
    if doThing:
        data = line.split(' ')
        current.append(list(map(int, data)))

def create_ranges(map):
    src, dst = [], []

    for map in maps:
        for triple in map:
            src.append((triple[1], triple[1] + triple[2] - 1))
            dst.append((triple[0], triple[0] + triple[2] - 1))

        yield src, dst
        src, dst = [], []

for src, dst in create_ranges(maps):
    for i, seed in enumerate(seeds):
        for j, s in enumerate(src):
            if seed in range(s[0], s[1] + 1):
                seeds[i] = dst[j][0] + seed - s[0]
                break

print(f"1: {min(seeds)}")
