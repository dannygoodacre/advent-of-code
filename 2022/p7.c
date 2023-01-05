#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISK_SPACE 70000000
#define REQ_SPACE 30000000

typedef struct Node
{
    char *name; // Assumed unique (this is true for the input).
    size_t size; // 0 if directory.
    int nk; // Number of kids.
    struct Node *parent; // Superdirectory.
    struct Node **kids; // Subdirectories.
} Node;

// Get node in parent with name.
Node *getnode(Node *parent, char *name)
{
    for (int i = 0; i < parent->nk; i++)
        if (!strcmp(parent->kids[i]->name, name))
            return parent->kids[i];
}

// Add node to parent.
void addnode(Node *parent, size_t size, char *name)
{
    // Create node and fill in data.
    Node *node = malloc(sizeof(Node));
    node->kids = malloc(20 * sizeof(Node*));
    node->name = malloc(20 * sizeof(char));
    strcpy(node->name, name);
    node->size = size;
    node->nk = 0;
    node->parent = parent;
    // Add node to parent.
    parent->kids[parent->nk++] = node;
}

void displaycontents(Node *node)
{
    for (int i = 0; i < node->nk; i++)
        printf("%lu %s\n", node->kids[i]->size, node->kids[i]->name);
}

// Get size of Node (including any sub-directories).
size_t getsize(struct Node *node)
{
    if (node->size > 0)
        return node->size;
    size_t size = 0;
    for (int i = 0; i < node->nk; i++)
        size += getsize(node->kids[i]); // We must go deeper!
    return size;
}

// Return true if node contains another directory.
int containsdir(struct Node *node)
{
    for (int i = 0; i < node->nk; i++)
        if (!node->kids[i]->size)
            return 1;
    return 0;
}

// Put sum of directory sizes less than 100,000 in size.
void getdirsizes(struct Node *node, size_t *size)
{
    for (int i = 0; i < node->nk; i++)
    {
        if (!node->kids[i]->size)
        {
            if (containsdir(node->kids[i]))
                getdirsizes(node->kids[i], size);
            if (getsize(node->kids[i]) < 100000)
                *size += getsize(node->kids[i]);
        }
    }
}

// Return smallest directory that is large enough.
size_t spam(Node *node, size_t size)
{
    size_t min = 0, current;
    
    for (int i = 0; i < node->nk; i++)
        if (!node->kids[i]->size) // Is a directory.
        {
            current = getsize(node->kids[i]);
            if ((current < min || !min) && current >= size)
                min = current;
        }
    return min;
}

// Put minimum viable directory size in min.
void getmin(Node *node, size_t *min, size_t size)
{
    for (int i = 0; i < node->nk; i++)
    {
        if (!node->kids[i]->size) // Is a directory.
            getmin(node->kids[i], min, size);
    }
    size_t s = getsize(node);
    if (s >= size && (s < *min || !*min))
        *min = s;
}

int main()
{
    // Root does not have a parent, and does not need one for the input.
    Node root = {"/", 0, 0, malloc(sizeof(Node)), malloc(20 * sizeof(Node*))};
    Node *cwd = &root;

    FILE *f = fopen("input.txt", "r");
    char *line, *input = malloc(10 * sizeof(char));
    size_t len, size;
    while (getline(&line, &len, f) != -1)
    {
        if (sscanf(line, "$ cd %s", input) > 0)
        {
            if (!strncmp(input, "..", 2))
                cwd = cwd->parent;
            else if (!strncmp(input, "/", 1))
                continue;
            else
                cwd = getnode(cwd, input);
        }
        else if (sscanf(line, "dir %s", input) > 0)
            addnode(cwd, 0, input);
        else if (sscanf(line, "%lu %s", &size, input))
            addnode(cwd, size, input);
    }
    fclose(f);

    size_t need_space = REQ_SPACE - DISK_SPACE + getsize(&root);
    size_t answer1 = 0, answer2 = 0;
    getdirsizes(&root, &answer1);
    getmin(&root, &answer2, need_space);
    printf("1: %lu\n2: %lu\n", answer1, answer2);

    // TODO: free rest of memory used.
    free(cwd);
    free(line);
    free(input);
    return 0;
}