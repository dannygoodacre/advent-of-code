#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    int x, y, d; // d = distance from start. -1 means unvisited.
    struct Node *parent;
} Node;

Node **nodes;
char **height_map;
int height, width;

typedef struct Queue
{
    size_t start, end, n; // n = no. of elements.
    Node *items[10000];
} Queue;

void push(Queue *queue, Node *item)
{
    queue->items[++queue->end] = item;
    queue->n++;
}

Node *pop(Queue *queue)
{
    queue->n--;
    return queue->items[queue->start++];
}

int compare(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

// Read elevation data from input file and get height and width.
void get_height_map()
{
    FILE *f = fopen("input.txt", "r");
    char *line = NULL;
    size_t len;
    for (height = 0; getline(&line, &len, f) != -1; height++);
    width = strlen(line) - 1;

    height_map = malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++)
        height_map[i] = malloc(width * sizeof(char));
    rewind(f);
    for (int i = 0; getline(&line, &len, f) != -1; i++)
        strcpy(height_map[i], line);
    fclose(f);
}

// Put start and end points in given variables.
void find_start_end(int *xstart, int *ystart, int *xend, int *yend)
{
    FILE *f = fopen("input.txt", "r");
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            if (height_map[i][j] == 'S')
                height_map[*ystart = i][*xstart = j] = 'a';
            if (height_map[i][j] == 'E')
                height_map[*yend = i][*xend = j] = 'z';
        }
}

// Establish grid of nodes representing points on the map.
void setup_nodes()
{
    nodes = malloc(height * sizeof(Node*));
    for (int i = 0; i < height; i++)
    {
        nodes[i] = malloc(width * sizeof(Node));
        for (int j = 0; j < width; j++)
        {
            nodes[i][j].x = j;
            nodes[i][j].y = i;
            nodes[i][j].d = -1;
        }
    }
}

// Mark all nodes as unvisited.
void reset_nodes()
{
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            nodes[i][j].d = -1;
}

// Return true if point is in range of grid.
int is_in_range(int x, int y)
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

// Return true if point is unvisited.
int is_new(int x, int y, int i, int j)
{
    return is_in_range(x + i, y + j)
    && (!i ^ !j) // adjacent, not diagonal
    && height_map[y + j][x + i] - height_map[y][x] <= 1 // one step up at most
    && nodes[y + j][x + i].d == -1; // unvisited
}

// Return shortest distance from start to finish using a breadth-first search.
int bfs_shortest_dist(int xstart, int ystart, int xend, int yend)
{
    Queue q = {0, -1, 0};
    nodes[ystart][xstart].d = 0;
    nodes[ystart][xstart].parent = NULL; // No parent for starting point.
    push(&q, &nodes[ystart][xstart]);
    while (q.n) // Queue is not empty.
    {
        Node *node = pop(&q);
        if (node->x == xend && node->y == yend) // Found!
            return node->d;
        // For all unvisited points around node, mark visited and add to queue.
        for (int i = -1; i < 2; i++)
            for (int j = -1; j < 2; j++)
                if (is_new(node->x, node->y, i, j))
                {
                    nodes[node->y + j][node->x + i].d = node->d + 1;
                    nodes[node->y + j][node->x + i].parent = node;
                    push(&q, &nodes[node->y + j][node->x + i]);
                }
    }
    return -1; // Not found.
}

// Find the starting point on the given elevation such that the distance from
// that point to the end point is minimised, and return that distance.
int bfs_shortest_dist_from_elevation(char elevation, int xend, int yend)
{
    Node *starts = malloc(sizeof(Node));
    int n = 0;
    for (int i = 0; i < height; i++) // Find all points on given elevation.
        for (int j = 0; j < width; j++)
            if (height_map[i][j] == elevation)
            {
                starts[n].x = j;
                starts[n].y = i;
                starts[n].d = -1;
                starts[n++].parent = NULL;
                starts = realloc(starts, (n + 1) * sizeof(Node));
            }

    int *distances = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        reset_nodes();
        distances[i] = bfs_shortest_dist(starts[i].x, starts[i].y, xend, yend);
    }
    free(starts);
    // Not every point is reachable, so we skip these.
    // Return the first distance in the sorted array that is not -1.
    qsort(distances, n, sizeof(int), compare);
    int i = 0;
    for (i = 0; distances[i] == -1; i++);
    return distances[i];
}

int main()
{
    int xstart, ystart, xend, yend;
    get_height_map();
    find_start_end(&xstart, &ystart, &xend, &yend);
    setup_nodes();
    printf("1: %d\n", bfs_shortest_dist(xstart, ystart, xend, yend));
    printf("2: %d\n", bfs_shortest_dist_from_elevation('a', xend, yend));
    return 0;
}