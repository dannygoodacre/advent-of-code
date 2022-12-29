#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int priorityval(char c)
{
    if (!c)
        return 0;
    return c - ((c > 96 && c < 123) ? 96 : 38);
}

int countc(char *str)
{
    int i = 0;
    for (; str[i] != '\n'; i++);
    return i;
}

char findintwo(char *first, char *second, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (first[i] == second[j])
                return second[j];
    return 0;
}

char findinthree(char *first, char *second, char *third, int n1, int n2, int n3)
{
    char c;
    for (int i = 0; first[i]; i++)
        for (int j = 0; second[j]; j++)
            if ((c = first[i]) == second[j])
                for (int k = 0; third[k]; k++)
                    if (c == third[k])
                        return c;
}

int main()
{
    FILE *f = fopen("input.txt", "r");
    char *line, *first, *second, **lines = malloc(3 * sizeof(char*));
    size_t len, n;
    int score1 = 0, score2 = 0, e = 0;

    while (getline(&line, &len, f) != -1)
    {
        // 1
        n = countc(line) / 2;
        first = realloc(first, n + 1);
        first[n] = 0;
        second = realloc(second, n + 1);
        second[n] = 0;
        for (int i = 0; i < n; i++)
        {
            first[i] = line[i];
            second[i] = line[n + i];
        }
        char c = findintwo(first, second, n);
        score1 += priorityval(c);
        // 2
        lines[e] = realloc(lines[e], 2*n + 1);
        strcpy(lines[e++], line);
        if (e == 3)
        {
            e = 0;
            char c = findinthree(lines[e], lines[e + 1], lines[e + 2], 
                countc(lines[e]), countc(lines[e + 1]), countc(lines[e + 2]));
            score2 += priorityval(c);
        }
    }
    printf("1: %d\n", score1);
    printf("2: %d\n", score2);

    fclose(f);
    free(line);
    free(first);
    free(second);
    free(lines[0]);
    free(lines[1]);
    free(lines[2]);
    free(lines);
    return 0;
}