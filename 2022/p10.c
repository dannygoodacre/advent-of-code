#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Print # or . based on cycle and register x.
void draw(int cycle, int x)
{
    int pos = (cycle - 1) % 40;
    printf("%c", abs(x - pos) < 2 ? '#' : '.');
    if (!(cycle % 40)) 
        printf("\n");
}

int main()
{
    FILE *f = fopen("input.txt", "r");
    char *line;
    int n, x = 1, cycle = 1, answer1 = 0;
    size_t len;

    printf("2:\n");
    while (getline(&line, &len, f) != -1)
    {
        sscanf(line, "%*s %d", &n);
        draw(cycle, x);
        if (!((cycle + 20) % 40))
                answer1 += cycle * x;
        if (line[0] == 'a') // addx n
        {
            draw(++cycle, x);
            if (!((cycle + 20) % 40))
                answer1 += cycle * x;
            x += n;
        }
        cycle++;
    }
    printf("1: %d\n", answer1);

    fclose(f);
    free(line);
    return 0;
}