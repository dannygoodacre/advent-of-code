#include <stdio.h>
#include <stdlib.h>

int isunique(char *str, int n);

int main()
{
    FILE *f = fopen("input.txt", "r");
    char *line;
    size_t len;
    int answer1 = -1, answer2 = -1;
    getline(&line, &len, f);
    for (int i; line[i]; i++)
    {
        if (isunique(&line[i], 4) && answer1 == -1)
            answer1 = i + 4;
        if (isunique(&line[i], 14) && answer2 == -1)
            answer2 = i + 14;
    }
    printf("1: %d\n2: %d\n", answer1, answer2);
    fclose(f);
    free(line);
    return 0;
}

int isunique(char *str, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = i+1; j < n; j++)
            if (str[i] == str[j])
                return 0;
    return 1;
}