#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Pos
{
    int x;
    int y;
} Pos;

// Return chessboard distance (discrete Chebyshev distance) between a and b.
int dist(Pos *a, Pos *b)
{
    return (int) sqrt((a->x-b->x)*(a->x-b->x) + (a->y-b->y)*(a->y-b->y));
}

// Move p one step in direction d.
void move(Pos *p, char d)
{
    switch (d)
    {
        case 'L':
            p->x--;
            break;
        case 'R':
            p->x++;
            break;
        case 'U':
            p->y++;
            break;
        case 'D':
            p->y--;
            break;
    }
}

// Move tail t based on position of head h.
void follow(Pos *t, Pos *h)
{
    // Since d > 1, we know that x and y coords are never both equal.
    // No movement if touching or on top of each other (d <= 1).
    if (dist(t, h) > 1) 
    {
        if (h->x == t->x)
            t->y += h->y > t->y ? 1 : -1;
        else
        {
            t->x += h->x > t->x ? 1 : -1;
            t->y += h->y != t->y ? h->y > t->y ? 1 : -1 : 0;
        }
    }
}

// Return true if p is already in visited.
int hasvisited(Pos *p, Pos *visited, int nvisited)
{
    for (int i = 0; i < nvisited; i++)
        if (p->x == visited[i].x && p->y == visited[i].y)
            return 1;
    return 0;
}

int main()
{
    FILE *f = fopen("input.txt", "r");
    char *line, d;
    size_t len;
    int n, nvisited = 0;

    // Part 1
    Pos h = {0, 0}, t = {0, 0}, *visited = malloc(sizeof(Pos));
    visited[nvisited++] = t;
    while (getline(&line, &len, f) != -1)
    {
        sscanf(line, "%c %d", &d, &n);
        for (int i = 0; i < n; i++)
        {
            move(&h, d);
            follow(&t, &h);
            if (!hasvisited(&t, visited, nvisited))
            {
                // Add tail position to visited.
                visited = realloc(visited, (nvisited + 1) * sizeof(Pos));
                visited[nvisited++] = t; 
            }
        }
    }
    printf("1: %d\n", nvisited);

    // Part 2
    rewind(f);
    nvisited = 0;
    free(visited);
    visited = malloc(sizeof(Pos));
    Pos knots[10];
    // All knots are initially at the origin.
    for (int i = 0; i < 10; i++)
    {
        knots[i].x = 0;
        knots[i].y = 0;
    }
    visited[nvisited++] = knots[9];
    // knots[0] = head, knots[9] = tail. This approach can be adapted to any
    // number of knots.
    while (getline(&line, &len, f) != -1)
    {
        sscanf(line, "%c %d", &d, &n);
        for (int i = 0; i < n; i++)
        {            
            move(&knots[0], d);
            for (int j = 1; j < 10; j++) 
                follow(&knots[j], &knots[j - 1]);

            if (!hasvisited(&knots[9], visited, nvisited))
            {
                visited = realloc(visited, (nvisited + 1) * sizeof(Pos));
                visited[nvisited++] = knots[9];
            }
        }
    }
    printf("2: %d\n", nvisited);

    fclose(f);
    free(line);
    free(visited);
    return 0;
}