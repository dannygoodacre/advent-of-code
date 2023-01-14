#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b)
{
    return *(int*)b - *(int*)a;
}

typedef struct Queue
{
    int start;
    int end;
    int n;
    size_t items[1000000];
} Queue;

typedef struct Monkey
{
    Queue *items;
    char op;
    char opn[10];
    int testn;
    int monkeytrue;
    int monkeyfalse;
    int ninspects;
} Monkey;

void push(Queue *queue, size_t item)
{
    queue->items[++queue->end] = item;
    queue->n++;
}

int pop(Queue *queue)
{
    // if (!queue->n)
    //     return -1; // All values for our purposes are non-negative.
    queue->n--;
    return queue->items[queue->start++];
}

// Get items from string and add them to monkey's items.
void getitems(Monkey monkey, char *items)
{
    for (char *p = strtok(items, ", "); p; p = strtok(NULL, ", "))
        push(monkey.items, atoi(p));
}

// Return the index of the monkey to whom the current item must be thrown.
int test(Monkey m)
{
    return !(m.items->items[m.items->start] % m.testn) 
        ? m.monkeytrue : m.monkeyfalse;
}

// Inspect current item, changing value accordingly.
void inspect(Monkey *m)
{
    int val = atoi(m->opn);
    if (!strcmp(m->opn, "old"))
        val = m->items->items[m->items->start];
    switch (m->op)
    {
        case '+':
            m->items->items[m->items->start] += val;
            break;
        case '*':
            m->items->items[m->items->start] *= val;
    }
    m->ninspects++;
}

// Change worry level of current item..
void getbored(Monkey *m, size_t s)
{
    if (s)
        m->items->items[m->items->start] %= s;
    else
        m->items->items[m->items->start] /= 3;
}

// Move current item.
void throw(Monkey *from, Monkey *to)
{
    push(to->items, pop(from->items));
}

// Process all of the items belonging to monkey.
void taketurn(Monkey *monkeys, int monkeyn, int n, size_t s)
{
    Monkey *m = &monkeys[n];
    for (int i = m->items->n; m->items->n; i++)
    {
        inspect(m);
        getbored(m, s);
        throw(m, &monkeys[test(*m)]);
    }
}

int main()
{
    FILE *f = fopen("input.txt", "r");
    char *line, op, opn[10];
    size_t len, s = 1;
    int monkeyn;
    Monkey monkeys[10];
    for (int i = 0; i < 10; i++)
        monkeys[i].items = malloc(sizeof(Queue));

    // Part 2
    for (int i = 0; getline(&line, &len, f) != -1; i++)
    {
        switch (i % 7)
        {
            case 0: // Monkey.
                sscanf(line, "Monkey %d:\n", &monkeyn);
                monkeys[monkeyn].items->start = 0;
                monkeys[monkeyn].items->end = -1;
                monkeys[monkeyn].items->n = 0;
                monkeys[monkeyn].ninspects = 0;
                break;
            case 1: // Starting items.
                getitems(monkeys[monkeyn], strchr(line, ':') + 2);
                break;
            case 2: // Operation.
                sscanf(strchr(line, 'd') + 2, "%c %s\n", &op, opn);
                monkeys[monkeyn].op = op;
                strcpy(monkeys[monkeyn].opn, opn);
                break;
            case 3: // Divisibility test.
                monkeys[monkeyn].testn = atoi(strchr(line, 'y') + 2);
                s *= monkeys[monkeyn].testn;
                break;
            case 4: // Monkey if true.
                monkeys[monkeyn].monkeytrue = atoi(strchr(line, 'y') + 2);
                break;
            case 5: // Monkey if false.
                monkeys[monkeyn].monkeyfalse = atoi(strchr(line, 'y') + 2);
        }
    }
    monkeyn++; // Total no. of monkeys.

    for (int i = 0; i < 10000; i++)
        for (int j = 0; j < monkeyn; j++)
            taketurn(&monkeys[0], monkeyn, j, s);

    size_t *inspects = malloc(monkeyn * sizeof(size_t));
    for (int i = 0; i < monkeyn; i++)
        inspects[i] = monkeys[i].ninspects;
    qsort(inspects, monkeyn, sizeof(size_t), compare);
    printf("2: %lu\n", inspects[0] * inspects[1]);

    // Part 1
    rewind(f);
    monkeyn = 0;
    for (int i = 0; i < 10; i++)
        monkeys[i].items = malloc(sizeof(Queue));
    // Get input data.
    for (int i = 0; getline(&line, &len, f) != -1; i++)
    {
        switch (i % 7)
        {
            case 0: // Monkey.
                sscanf(line, "Monkey %d:\n", &monkeyn);
                monkeys[monkeyn].items->start = 0;
                monkeys[monkeyn].items->end = -1;
                monkeys[monkeyn].items->n = 0;
                monkeys[monkeyn].ninspects = 0;
                break;
            case 1: // Starting items.
                getitems(monkeys[monkeyn], strchr(line, ':') + 2);
                break;
            case 2: // Operation.
                sscanf(strchr(line, 'd') + 2, "%c %s\n", &op, opn);
                monkeys[monkeyn].op = op;
                strcpy(monkeys[monkeyn].opn, opn);
                break;
            case 3: // Divisibility test.
                monkeys[monkeyn].testn = atoi(strchr(line, 'y') + 2);
                s *= monkeys[monkeyn].testn;
                break;
            case 4: // Monkey if true.
                monkeys[monkeyn].monkeytrue = atoi(strchr(line, 'y') + 2);
                break;
            case 5: // Monkey if false.
                monkeys[monkeyn].monkeyfalse = atoi(strchr(line, 'y') + 2);
        }
    }
    fclose(f);
    free(line);
    monkeyn++; // Total no. of monkeys.

    for (int i = 0; i < 20; i++)
        for (int j = 0; j < monkeyn; j++)
            taketurn(&monkeys[0], monkeyn, j, 0);

    for (int i = 0; i < monkeyn; i++)
        inspects[i] = monkeys[i].ninspects;
    qsort(inspects, monkeyn, sizeof(size_t), compare);
    printf("1: %lu\n", inspects[0] * inspects[1]);

    for (int i = 0; i < 10; i++)
        free(monkeys[i].items);
    free(inspects);
    return 0;
}