#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int containsdigit(char *line);
int countcaps(char *line);
char strpop(char *str);
char *strpopn(char *str, int n);
void popn(char *src, char *dest, int n);
void moven(char *src, char *dest, int n);

int main()
{
    FILE *f;
    char *line;
    size_t len;
    int i;

    // Get no. of lines, stacks, and crates.
    int nl = 0, ns, nc = 0;
    f = fopen("input.txt", "r");
    while (getline(&line, &len, f) != -1)
    {
        nc += countcaps(line);
        if (containsdigit(line))
        {
            ns = line[strlen(line) - 3] - 48;
            break;
        }
        nl++;
    }
    fclose(f);

    // Get lines containing crates in reverse order.
    char **lines = malloc(nl * sizeof(char*));
    f = fopen("input.txt", "r");
    for (i = 0; i < nl; i++)
    {
        getline(&line, &len, f);
        lines[nl - 1 - i] = malloc(strlen(line) * sizeof(char));
        strcpy(lines[nl - 1 - i], line);
    }
    fclose(f);

    // Allocate enough space in each stack for all of the crates.
    char **stacks1 = malloc(ns * sizeof(char*));
    char **stacks2 = malloc(ns * sizeof(char*));
    for (i = 0; i < ns; i++)
    {
        stacks1[i] = calloc(nc, sizeof(char));
        stacks2[i] = calloc(nc, sizeof(char));
    }
    
    // Fill in stacks.
    char c;
    for (i = 0; i < nl; i++)
        for (int j = 0; j < ns; j++)
            if ((c = lines[i][4*j + 1]) != ' ')
            {
                stacks1[j][i] = c;
                stacks2[j][i] = c;
            }

    // Skip to instructions.
    f = fopen("input.txt", "r");
    for (i = 0; i < nl+2; i++)
        getline(&line, &len, f);

    // Store instruction lines.
    char **instructions = malloc(sizeof(char*));
    for (i = 0; getline(&line, &len, f) != -1; i++)
    {
        instructions[i] = malloc(strlen(line) * sizeof(char));
        instructions = realloc(instructions, (i + 2) * sizeof(char*));
        strcpy(instructions[i], line);
    }
    fclose(f);

    // Get relevant data from instructions.
    int data[i][3];
    for (i = 0; instructions[i]; i++)
    {
        strtok(instructions[i], " ");
        data[i][0] = atoi(strtok(NULL, " "));
        strtok(NULL, " ");
        data[i][1] = atoi(strtok(NULL, " "));
        strtok(NULL, " ");
        data[i][2] = atoi(strtok(NULL, " "));
        popn(stacks1[data[i][1] - 1], stacks1[data[i][2] - 1], data[i][0]);
        moven(stacks2[data[i][1] - 1], stacks2[data[i][2] - 1], data[i][0]);
    }

    char *answer1 = malloc(ns * sizeof(char));
    char *answer2 = malloc(ns * sizeof(char));
    for (i = 0; i < ns; i++)
    {
        answer1[i] = strpop(stacks1[i]);
        answer2[i] = strpop(stacks2[i]);
    }
    answer1[i] = 0;
    answer2[i] = 0;
    printf("1: %s\n2: %s\n", answer1, answer2);

    free(line);
    for (i = 0; i < nl; i++)
        free(lines[i]);
    free(lines);
    for (i = 0; i < ns; i++)
        free(stacks1[i]);
    free(stacks1);
    for (i = 0; i < ns; i++)
        free(stacks2[i]);
    free(stacks2);
    for (i = 0; instructions[i]; i++)
        free(instructions[i]);
    free(instructions);
    free(answer1);
    free(answer2);
    return 0;
}

// Return true if line contains digit.
int containsdigit(char *line)
{
    char c;
    for (int i = 0; c = line[i]; i++)
        if (isdigit(c))
            return 1;
    return 0;
}

// Return no. of capital letters (A-Z) in line.
int countcaps(char *line)
{
    int count = 0;
    char c;
    for (int i = 0; c = line[i]; i++)
        if (c > 64 && c < 91)
            count++;
    return count;
}

// Return and remove last char from str.
char strpop(char *str)
{
    char c;
    int i;
    for (i = 0; str[i]; i++);
    c = str[--i];
    str[i] = 0;
    return c;
}

// Return and remove last n chars in str.
char *strpopn(char *str, int n)
{
    char *s = malloc((n + 1) * sizeof(char));
    s[n] = 0;
    for (int i = 0; i < n; i++)
        s[i] = str[i + strlen(str) - n];
    str[strlen(str) - n] = 0;
    return s;
}

// Move last n chars in src to end of dest (LIFO).
void popn(char *src, char *dest, int n)
{
    char c[2] = {0, 0};
    for (int i = 0; i < n; i++)
    {
        c[0] = strpop(src);
        dest = strcat(dest, c);
    }
}

// Move last n chars in src to end of dest (FIFO).
void moven(char *src, char *dest, int n)
{
    char *s = strpopn(src, n);
    dest = strcat(dest, s);
    free(s);
}