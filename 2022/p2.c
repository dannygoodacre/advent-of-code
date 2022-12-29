#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *f = fopen("input.txt", "r");
    char *line;
    size_t len;
    int score1 = 0, score2 = 0, val;

    while (getline(&line, &len, f) != -1)
    {
        // 1
        score1 += line[2] - 87;
        if ((val = line[2] - line[0]) == 23)
            score1 += 3;
        else if (!(val % 3))
            score1 += 6;
        val = line[0] - 64;
        // 2
        if (line[2] == 'Y')
            score2 += val + 3;
        if (line[2] == 'X')
            score2 += (val == 1) ? 'C' - 64 : val - 1;
        if (line[2] == 'Z')
            score2 += ((val == 3) ? 'A' - 64: val + 1) + 6;
    }
    printf("1: %d\n", score1);
    printf("2: %d\n", score2);

    free(line);
    fclose(f);
    return 0;
}