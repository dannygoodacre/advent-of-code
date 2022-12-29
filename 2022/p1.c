#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b)
{
    return *(int*)b - *(int*)a;
}

int main()
{
    FILE *f = fopen("input.txt", "r");
    char *line;
    size_t len;
    int value = 0, n = 0;
    int *calories = malloc(sizeof(int));

    while (getline(&line, &len, f) != -1)
    {
        if (line[0] != '\n')
            value += atoi(line);
        else
        {
            calories[n++] = value;
            calories = realloc(calories, (n + 1) * sizeof(int));
            value = 0;
        }
    }
    calories[n] = value;
    qsort(calories, ++n, sizeof(int), compare);
    printf("1: %d\n", calories[0]);
    printf("2: %d\n", calories[0] + calories[1] + calories[2]);

    fclose(f);
    free(line);
    free(calories);
    return 0;
}