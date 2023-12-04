red = 12
green = 13
blue = 14

def is_valid(n, s):
    return (s != "red" or n <= red) \
        and (s != "green" or n <= green) \
        and (s != "blue" or n <= blue)

total1 = 0
total2 = 0

for line in open("input.txt").read().splitlines():
    max_red = 0
    max_green = 0
    max_blue = 0

    id = line[5:line.find(':')]
    line = line[line.find(':') + 1:]
    sets = [set.strip() for set in line.split(';')]
    split_sets = [set.split(', ') for set in sets]

    valid = True
    for set in split_sets:

        for pair in set:
            n, s = pair.split(' ')
            n = int(n)

            if s == "red" and n > max_red:
                max_red = n
            elif s == "green" and n > max_green:
                max_green = n
            elif s == "blue" and n > max_blue:
                max_blue = n

            # Once one value is found to be invalid, ignore result for rest.
            if not is_valid(n, s):
                valid = False

    total1 += valid * int(id)
    total2 += max_red * max_green * max_blue

print("1: " + str(total1) + "\n2: " + str(total2))
