#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Return column i of data.
char *getcol(char **data, int i, int dim)
{
    char *col = malloc(dim * sizeof(char));
    for (int j = 0; j < dim; j++)
        col[j] = data[j][i];
    return col;
}

// Return true if pos i in row is visible from left.
int visleft(char *row, int i, int dim)
{
    int max = row[0];
    int mi = 0;
    for (int j = 1; j < i; j++)
        max = max < row[j] ? row[j] : max;
    return row[i] > max;
}

// Return true if pos i in row is visible from right.
int visright(char *row, int i, int dim)
{
    int max = row[i+1];
    for (int j = i + 1; j < dim; j++)
        max = max < row[j] ? row[j] : max;
    return row[i] > max;
}

// Return true if visible from any side.
int isvisible(char **data, int i, int j, int dim)
{
    return visleft(data[i], j, dim) || visright(data[i], j, dim) 
        || visleft(getcol(data, j, dim), i, dim) 
        || visright(getcol(data, j, dim), i, dim);
}

// Return distance to blocking tree (left).
int dleft(char *row, int i)
{
    for (int j = i - 1; j >= 0; j--)
        if (row[j] >= row[i])
            return i - j;
    return i;
}

// Return distance to blocking tree (right).
int dright(char *row, int i, int dim)
{
    for (int j = i + 1; j < dim; j++)
        if (row[j] >= row[i])
            return j - i;
    return dim - i - 1;
}

// Return scenic score of pos (i,j) in data.
int getscore(char **data, int i, int j, int dim)
{
    return dleft(data[i], j) * dright(data[i], j, dim) 
        * dleft(getcol(data, j, dim), i) * dright(getcol(data, j, dim), i, dim);
}

int compare(const void *a, const void *b)
{
    return *(int*)b - *(int*)a;
}

int main()
{
    FILE *f = fopen("input.txt", "r");
    char *line;
    size_t len;
    getline(&line, &len, f);
    int dim = strlen(line) - 1;
    rewind(f);
    char **data = malloc(dim * sizeof(char*));
    for (int i = 0; getline(&line, &len, f) != -1; i++)
        strcpy(data[i] = malloc(dim * sizeof(char)), line);
    fclose(f);

    int count = 0, n = 0, *score = malloc((dim - 2) * (dim - 2) * sizeof(int));
    for (int i = 1; i < dim - 1; i++)
        for (int j = 1; j < dim - 1; j++)
        {
            count += isvisible(data, i, j, dim);
            score[n++] = getscore(data, i, j, dim);
        }

    qsort(score, ++n, sizeof(int), compare);
    printf("1: %d\n2: %d\n", dim*dim - (dim - 2)*(dim - 2) + count, score[0]);

    free(line);
    for (int i = 0; i < dim; i++)
        free(data[i]);
    free(data);
    free(score);
    // TODO: free rest of data. 
    // getcol is the likely culprit.
    return 0;
}