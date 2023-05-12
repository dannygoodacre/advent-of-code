#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define X0 500
#define Y0 0

// Replace every occurence of old with new in str, in place.
void replace(char* s, char* old, char* new)
{
    char *pos, temp[1000];
    int index = 0;
    int oldlen = strlen(old);

    while ((pos = strstr(s, old)) != NULL) {
        strcpy(temp, s);
        s[index = pos - s] = 0;
        strcat(s, new); 
        strcat(s, temp + index + oldlen);
    }
}

// Put maximum x and y dimensions in xmax and ymax.
void get_size(int *xmax, int *ymax)
{
    FILE *f = fopen("input.txt", "r");
    char *line = NULL, *s, *temp;
    size_t len;
    int x, y;
    *xmax = 0, *ymax = 0; 

    for (int i = 0; getline(&line, &len, f) != -1; i++)
    {
        replace(line, " -> ", ":");
        s = strtok(line, ":");
        do
        {
            if ((x = strtol(strtok_r(s, ",", &temp), NULL, 10)) > *xmax)
                *xmax = x;
            if ((y = strtol(strtok_r(NULL, ",", &temp), NULL, 10)) > *ymax)
                *ymax = y;
        } 
        while ((s = strtok(NULL, ":")) != NULL);
    }
    (*xmax)++, (*ymax)++;

    fclose(f);
    free(line);
    free(s);
}

// Insert rocks into grid, reading positions from file.
void insert_rocks(char ***grid)
{
    FILE *f = fopen("input.txt", "r");
    char *line = NULL, *s, *temp;
    size_t len;
    int count, j, *x, *y;

    for (int i = 0; getline(&line, &len, f) != -1; i++)
    {
        // Get number of coordinates.
        count = 1;
        for (int i = 0; line[i]; count += line[i++] == '-');
        
        // Read coordinates from file and store in x and y.
        x = malloc(count * sizeof(int)), y = malloc(count * sizeof(int));
        replace(line, " -> ", ":");
        s = strtok(line, ":");
        j = 0;
        do
        {
            x[j] = strtol(strtok_r(s, ",", &temp), NULL, 10);
            y[j++] = strtol(strtok_r(NULL, ",", &temp), NULL, 10);
        }
        while ((s = strtok(NULL, ":")) != NULL);

        // Insert rocks at given coordinates.
        if (count == 1) // Single point.
            (*grid)[y[0]][x[0]] = '#';
        for (int i = 0; i < count - 1; i++) // Range of points.
        {
            if (x[i] != x[i + 1])
            {
                if (x[i] < x[i + 1])
                    for (int j = x[i]; j <= x[i + 1]; j++)
                        (*grid)[y[i]][j] = '#';
                else
                    for (int j = x[i + 1]; j <= x[i]; j++)
                        (*grid)[y[i]][j] = '#';
            }

            if (y[i] != y[i + 1])
            {
                if (y[i] < y[i + 1])
                    for (int j = y[i]; j <= y[i + 1]; j++)
                        (*grid)[j][x[i]] = '#';
                else
                    for (int j = y[i + 1]; j <= y[i]; j++)
                        (*grid)[j][x[i]] = '#';
            }
        }
    }
    fclose(f);
    free(line);
    free(s);
    free(x);
    free(y);
}

// Return true if position is in range of grid.
int is_valid(int x, int y, int xmax, int ymax)
{
    return x >= 0 && x < xmax && y >= 0 && y < ymax;
}

// Return true if specific position is clear: Left, Right, or Below.
int is_clear(char pos, char **grid, int x, int y, int xmax, int ymax)
{
    switch (pos)
    {
        case 'L':
            return is_valid(x - 1, y + 1, xmax, ymax) 
                && grid[y + 1][x - 1] == '.';
            break;
        case 'R':
            return is_valid(x + 1, y + 1, xmax, ymax) 
                && grid[y + 1][x + 1] == '.';
            break;
        case 'B':
            return is_valid(x, y + 1, xmax, ymax) && grid[y + 1][x] == '.';
    }
}

// Simulate dropping a unit of sand and return false if the criteria for 
// stopping - as specified in part 1 or 2 - is satisfied. Otherwise return true.
int drop(char ***grid, int xmax, int ymax, int part)
{
    int x = X0, y = Y0, below, left, right;

    do
    {
        below = is_clear('B', *grid, x, y, xmax, ymax);
        left = is_clear('L', *grid, x, y ,xmax, ymax);
        right = is_clear('R', *grid, x, y ,xmax, ymax);

        if (below)
            y++;
        else if (left)
        {
            y++;
            x--;
        }
        else if (right)
        {
            y++;
            x++;
        }
    } 
    while (below || left || right);

    if (part == 1 && y == ymax - 1 || part == 2 && (*grid)[Y0][X0] == 'o')
        return 0;
    (*grid)[y][x] = 'o';
    return 1;
}

// Write the grid to specified file.
void write_to_file(char **grid, int xmax, int ymax, char *name)
{
   FILE *f = fopen(name, "w");
    for (int i = 0; i < ymax; i++)
    {
        for (int j = 0; j < xmax; j++)
            fprintf(f, "%c", grid[i][j]);
        fprintf(f, "%c", '\n');
    }
    fclose(f); 
}

// Return solution to part 1.
int part1()
{
    // Get size of grid.
    int xmax, ymax;
    get_size(&xmax, &ymax);

    // Allocate space for grid and fill with air and rocks.
    char **grid = malloc(ymax * sizeof(char*));
    for (int i = 0; i < ymax; i++)
    {
        grid[i] = malloc(xmax * sizeof(char));
        for (int j = 0; j < xmax; j++)
            grid[i][j] = '.';
    }
    insert_rocks(&grid);

    // Simulate dropping sand until it begins falling into the abyss.
    int n = 0;
    while (drop(&grid, xmax, ymax, 1))
        n++;

    // write_to_file(grid, xmax, ymax, "out1.txt");

    for (int i = 0; i < ymax; i++)
        free(grid[i]);
    free(grid);
    return n;
}

// Return solution to part 2.
int part2()
{
    // Get size of grid.
    int xmax, ymax;
    get_size(&xmax, &ymax);
    xmax *= 2; // Enough width to be effectively infinite.
    ymax += 2;

    // Allocate space for grid and fill with air and rocks.
    char **grid = malloc(ymax * sizeof(char*));
    for (int i = 0; i < ymax; i++)
    {
        grid[i] = malloc(xmax * sizeof(char));
        for (int j = 0; j < xmax; j++)
            grid[i][j] = '.';
    }
    insert_rocks(&grid);

    // Insert floor.
    for (int i = 0; i < xmax - 1; i++)
        grid[ymax - 1][i] = '#';

    // Simulate dropping sand until it reaches the source.
    int n = 0;
    while (drop(&grid, xmax, ymax, 2))
        n++;

    // write_to_file(grid, xmax, ymax, "out2.txt");

    for (int i = 0; i < ymax; i++)
        free(grid[i]);
    free(grid);
    return n;
}

int main()
{
    printf("1: %d\n2: %d\n", part1(), part2());
    return 0;
}