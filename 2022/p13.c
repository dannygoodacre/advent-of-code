#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Return all but first char of string.
char *tail(char *s)
{
    return ++s;
}

// Return the three strings concatenated together.
char *join(const char *a, const char *b, const char *c) {
    return strcat(strcat(strcpy(malloc(strlen(a) + strlen(b) + strlen(c) +  1),
        a), b), c);
}

// Replace every occurence of old with new in str, in place.
void replace(char* s, char* old, char* new)
{
    char *pos, temp[1000];
    int index = 0;
    int oldlen = strlen(old);

    while ((pos = strstr(s, old)) != NULL) {
        strcpy(temp, s);
        s[index = pos - s] = 0;
        strcat(s, new); 
        strcat(s, temp + index + oldlen);
    }
}

// Remove newline from end and replace all occurences of 10 with A, in place.
void tidy(char *s)
{
    s[strcspn(s, "\n")] = 0;
    replace(s, "10", "A");
}

// Return true if packets are in the correct order.
int check(char *left, char *right)
{
    if (left[0] == right[0])
        return check(tail(left), tail(right));
    if (left[0] == ']')
        return 1;
    if (right[0] == ']')
        return 0;
    if (left[0] == '[')
        return check(tail(left), join((char[2]) {right[0], 0}, "]", tail(right)));
    if (right[0] == '[')
        return check(join((char[2]) {left[0], 0}, "]", tail(left)), tail(right));
    return left[0] < right[0];
}

// Return number of non-empty lines in file.
int count_lines(FILE *f)
{
    int lines = 0, c = 0, pc = '\n';

    while (c = fgetc(f), c != -1)
    {
        if (c == '\n' && pc != '\n')
            lines++;
        pc = c;
    }
    if (pc != '\n')
        lines++;
    return lines;
}

// Sort packets using check function.
void bsort(char **array, int size) 
{
    char *temp = malloc(1000 * sizeof(char));
    for (int step = 0; step < size - 1; step++)
    {    
        for (int i = 0; i < size - step - 1; i++)
        {  
            if (check(array[i + 1], array[i]))
            {
                strcpy(temp, array[i]);
                strcpy(array[i], array[i + 1]);
                strcpy(array[i + 1], temp);
            }
        }
    }
    free(temp);
}

int main()
{
    FILE *f = fopen("input.txt", "r");
    char *line;
    size_t len;
    char *first = malloc(1000 * sizeof(char));
    char *second = malloc(1000 * sizeof(char));

    int n = 0, total = 0;
    for (int i = 0; getline(&line, &len, f) != -1; i++)
    {
        switch (i % 3)
        {
            case 0:
                strcpy(first, line);
                tidy(first);
                break;
            case 1:
                strcpy(second, line);
                tidy(second);
                break;
            case 2:
                total += ++n * check(first, second);
        }
    }
    total += ++n * check(first, second);

    // Get number of lists (including dividers).
    rewind(f);
    n = count_lines(f) + 2;
    rewind(f);

    // Allocate space for lists.
    char **data = malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++)
        data[i] = malloc(1000 * sizeof(char));

    // Read lists into memory (including dividers).
    int i = 0;
    while (getline(&line, &len, f) != -1)
        if (line[0] != '\n')
        {
            tidy(line);
            strcpy(data[i++], line);
        }
    strcpy(data[n - 2], "[[2]]");
    strcpy(data[n - 1], "[[6]]");

    // Sort lists and find dividers.
    bsort(data, n);
    int a = 0, b = 0;
    for (int i = 0; i < n; i++)
    {
        if (a == 0 && !strcmp(data[i], "[[2]]"))
            a = i + 1;
        if (a != 0 && !strcmp(data[i], "[[6]]"))
            b = i + 1;
    }

    printf("1: %d\n2: %d\n", total, a * b);

    fclose(f);
    free(line);
    free(first);
    free(second);

    for (int i = 0; i < n; i++)
        free(data[i]);
    free(data);
    return 0;
}