#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *f = fopen("input.txt", "r");
    char *line;
    size_t len, count1 = 0, count2;
    char **data = malloc(2 * sizeof(char*));
    int *s = malloc(4 * sizeof(int));

    while (getline(&line, &len, f) != -1)
    {
        data[0] = strtok(line, ",");
        data[1] = strtok(NULL, ",");
        s[0] = atoi(strtok(data[0], "-"));
        s[1] = atoi(strtok(NULL, "-"));
        s[2] = atoi(strtok(data[1], "-"));
        s[3] = atoi(strtok(NULL, "-"));

        count1 += s[0] <= s[2] && s[1] >= s[3] || s[0] >= s[2] && s[1] <= s[3];
        count2 += s[1] >= s[2] && s[0] <= s[3];
    }

    printf("1: %zu\n2: %zu\n", count1, count2);
    fclose(f);
    free(line);
    free(data);
    free(s);
    return 0;
}