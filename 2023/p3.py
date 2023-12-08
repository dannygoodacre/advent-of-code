data = [list(row) for row in open("input.txt").read().splitlines()]
x_max = len(data[0])
y_max = len(data)
grid = [[None]*x_max for _ in range(y_max)]
id_grid = [[None]*x_max for _ in range(y_max)]
symbol_coords = []
gear_coords = []
next_id = 0

def get_numbers(row, y):
    global next_id
    start = 0
    number = ''

    for i in range(len(row) + 1):
        if i < len(row) and row[i].isdigit():
            if number == '':
                start = i
            number += row[i]

        elif number != '':
            for j in range(start, i):
                grid[y][j] = int(number)
                id_grid[y][j] = next_id
            start = 0
            number = ''
            next_id += 1

def get_symbol_coords(row, y):
    for i in range(len(row)):
        if not row[i].isdigit() and row[i] != '.':
            if row[i] == '*':
                gear_coords.append((y, i))
            symbol_coords.append((y, i))

def is_in_range(x, y):
    return x >= 0 and y >= 0 and x < x_max and y < y_max

def get_surrounding_points(pos):
    surrounding_points = []

    for i in range(-1, 2):
        for j in range(-1, 2):
            if is_in_range(pos[0] + i, pos[1] + j):
                surrounding_points.append((pos[0] + i, pos[1] + j))
    
    return surrounding_points

for y, row in enumerate(data):
    get_numbers(row, y)
    get_symbol_coords(row, y)

total1 = 0
total2 = 0

for pos in symbol_coords:
    surrounding_points = get_surrounding_points(pos)
    used_ids = []
    m = 0
    n = 0
    n_id = None
    
    for p in surrounding_points:
        if grid[p[0]][p[1]] is not None:
            if not id_grid[p[0]][p[1]] in used_ids:
                total1 += grid[p[0]][p[1]]
                used_ids.append(id_grid[p[0]][p[1]])

            if n == 0:
                n = grid[p[0]][p[1]]
                n_id = id_grid[p[0]][p[1]]

            if id_grid[p[0]][p[1]] != n_id:
                m = grid[p[0]][p[1]]
                break

    total2 += m * n

print(f"1: {total1}\n2: {total2}")