#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 99999

size_t count_lines(FILE *f)
{
    char *line = NULL;
    size_t len;
    size_t n;
    for (n = 0; getline(&line, &len, f) != -1; n++);
    free(line);
    rewind(f);
    return n;
}

// Replace all occurences of old with new, in place.
void strrep(char* s, char* old, char* new)
{
    char *pos, temp[strlen(s)];
    int index = 0, oldlen = strlen(old);

    while ((pos = strstr(s, old)) != NULL)
    {
        strcpy(temp, s);
        s[index = pos - s] = 0;
        strcat(s, new); 
        strcat(s, temp + index + oldlen);
    }

    free(pos);
}

// Return first occurence of name in names.
int get_index(char *name, char **names, size_t len)
{
    for (int i = 0; i < len; i++)
        if (!strcmp(name, names[i]))
            return i;
    return -1;
}

void parse_data(FILE *f, char **names, int *rates, char ***leads, int count)
{
    char *line = NULL;
    size_t len;
    for (int i = 0; getline(&line, &len, f) != -1; i++)
    {
        strrep(line, "Valve ", "");
        strrep(line, " has flow rate=", " ");
        strrep(line, "; tunnel leads to valve ", " ");
        strrep(line, "; tunnels lead to valves ", " ");
        strrep(line, ",", "");

        names[i] = strtok(line, " ");
        rates[i] = strtol(strtok(NULL, " "), NULL, 10);

        int j;
        char *token;
        for (j = 0; token = strtok(NULL, " "); j++)
        {
            strrep(token, "\n", "");
            leads[i][j] = token;
        }
        leads[i][++j] = 0;
        line = NULL;
    }
    free(line);
}

// Return adjacency matrix of graph.
int **adjacency_matrix(char **names, char ***leads, size_t n)
{
    int **adj_mat = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        adj_mat[i] = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        // Mark all distances as infinity, unless it is to itself.
        for (int j = 0; j < n; j++)
            adj_mat[i][j] = i == j ? 0 : INF;

        // Mark distances to all adjacent valves as 1.
        for (int j = 0; leads[i][j]; j++)
        {
            int k = get_index(leads[i][j], names, n);
            adj_mat[i][k] = 1;
            adj_mat[k][i] = 1;
        }
    }

    // Determine minimum distances between all valves using Floyd-Warshall.
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (adj_mat[i][k] + adj_mat[k][j] < adj_mat[i][j])
                    adj_mat[i][j] = adj_mat[i][k] + adj_mat[k][j];

    return adj_mat;
}

// Remove all zero-rate valves from data, excluding the starting valve.
int **remove_zeros(char **names, int *rates, int **adj_mat, size_t *n, int *start_pos)
{
    // Mark all distances associated with zero-rate valves.
    for (int i = 0; i < *n; i++)
    {
        for (int j = 0; j < *n; j++)
            if (!rates[i] && i != *start_pos)
            {
                adj_mat[i][j] = -1;
                adj_mat[j][i] = -1;
            }
    }

    int m = *n;

    // Remove zero-rate valves.
    for (int i = 0; i < *n; i++)
        if (!rates[i] && i != *start_pos)
        {
            int j = get_index(names[i], names, *n);
            memmove(names + j, names + j + 1, (*n - j) * sizeof(char*));
            memmove(rates + j, rates + j + 1, (*n - j) * sizeof(int));

            if (j < *start_pos)
                (*start_pos)--;

            (*n)--;
            i = 0;
        }

    // New adjacency matrix for non-zero-rate valves only.
    int **new_adj_mat;
    new_adj_mat = malloc(*n * sizeof(int*));
    for (int i = 0; i < *n; i++)
        new_adj_mat[i] = malloc(*n * sizeof(int));

    // Populate new adjacency matrix.
    int x = 0, y = 0, do_increment = 0;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
            if (adj_mat[j][i] != -1)
            {
                new_adj_mat[y][x++] = adj_mat[j][i];
                do_increment = 1;
            }
        if (do_increment)
            y++;
        do_increment = 0;
        x = 0;
    }

    for (int i = 0; i < *n; i++)
        free(adj_mat[i]);
    free(adj_mat);

    return new_adj_mat;
}

// Remove a from arr, returning new array.
int *remove_element(int *arr, int a, int len)
{
    int *new = malloc((len - 1) * sizeof(int));
    int k = 0;
    for (int j = 0; j < len; j++)
        if (arr[j] != a)
            new[k++] = arr[j];

    return new;
}

// Depth first search for path which maximises total pressure released.
int dfs(int **adj_mat, int *rates, int *remaining, int r_len, int time, int limit, int rate, 
    int flow, int pos)
{
    int max = rate + (limit - time)*flow;

    for (int i = 0; i < r_len; i++)
    {
        int r = remaining[i];
        int cost = adj_mat[pos][r] + 1;
        if (time + cost < limit) 
        {
            int *new_remaining = remove_element(remaining, r, r_len);
            int new_r_len = r_len - 1;
            int new_time = time + cost;
            int new_rate = rate + cost*flow;
            int new_flow = flow + rates[r];
            int new_pos = r;

            int possible_score = dfs(adj_mat, rates, new_remaining, new_r_len, 
                new_time, limit, new_rate, new_flow, new_pos);

            if (possible_score > max)
                max = possible_score;
            
            free(new_remaining);
        }
    }

    return max;
}

// Factorial.
long int fact(int n)
{
    return n > 1 ? n * fact(n - 1) : 1;
}

// How many ways to choose r elements from a set of n.
long int ncr(int n, int r)
{
    return fact(n) / (fact(r) * fact(n - r));
}

// Return true if a is in arr.
int is_in_arr(int *arr, int len, int a)
{
    for (int i = 0; i < len; i++)
        if (arr[i] == a)
            return 1;
    return 0;
}

// Return the set complement of arr in space.
int *comp(int *arr, int *space, int arr_len, int space_len)
{
    int *out = malloc((space_len - arr_len) * sizeof(int));
    int j = 0;
    for (int i = 0; i < space_len; i++)
        if (!is_in_arr(arr, arr_len, space[i]))
            out[j++] = space[i];

    return out;
}

// Hacky global for combinations. TODO: Find a better way around this.
int p = 0;

void combo_recur(int *arr, int data[], int start, int end, int index, int r, int **final)
{
    if (index == r)
    {
        for (int j = 0; j < r; j++)
            final[p][j] = data[j];
        p++;
        return;
    }
    for (int i = start; i <= end && end - i + 1 >= r - index; i++)
    {
        data[index] = arr[i];
        combo_recur(arr, data, i + 1, end, index + 1, r, final);
    }
}

// Return all possible combinations of length r from arr with length n.
int **combinations(int *arr, int n, int r)
{
    int **final = malloc(ncr(n, r) * sizeof(int*));
    for (int i = 0; i < ncr(n, r); i++)
        final[i] = malloc(r * sizeof(int));
    int data[r];
    combo_recur(arr, data, 0, n - 1, 0, r, final);
    p = 0;
    return final;
}

int main()
{
    char **names;
    int *rates;
    char ***leads;
    int **full_adj_mat;
    int **adj_mat;
    int *remaining;
    int start_pos;

    FILE *f = fopen("input.txt", "r");
    size_t n = count_lines(f);

    names = malloc(n * sizeof(char*));
    rates = malloc(n * sizeof(int));
    leads = malloc(n * sizeof(char**));
    for (int i = 0; i < n; i++)
        leads[i] = malloc(10 * sizeof(char*));

    parse_data(f, names, rates, leads, n);
    fclose(f);

    full_adj_mat = adjacency_matrix(names, leads, n);

    // Find starting position, so we don't remove it.
    for (int i = 0; i < n; i++)
        if (!strcmp(names[i], "AA"))
        {
            start_pos = i;
            break;
        }

    adj_mat = remove_zeros(names, rates, full_adj_mat, &n, &start_pos);

    remaining = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        remaining[i] = i;

    int time = 0;
    int pressure = 0;
    int flow = 0;

    printf("1: %d\n", dfs(adj_mat, rates, remaining, n, time, 30, pressure, flow, start_pos));

    for (int i = 0; i < n; i++)
        remaining[i] = i;

    int max_pressure = 0;
    for (int i = 1; i < n/2; i++)
    {
        // Look at every partition of non-zero valves in the space.
        int **c = combinations(remaining, n, i);
        for (int j = 0; j < ncr(n, i); j++)
        {
            // Maximise the pressures along each path.
            int max_a = dfs(adj_mat, rates, c[j], i, time, 26, pressure, flow, start_pos);
            int max_b = dfs(adj_mat, rates, comp(c[j], remaining, i, n), n - i, time, 26, pressure, 
                flow, start_pos);

            if (max_a + max_b > max_pressure)
                max_pressure = max_a + max_b;
        }
    }

    printf("2: %d\n", max_pressure);
    return 0;
}

// TODO: Fix the leaks and invalid reads.