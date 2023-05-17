#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YLEVEL 2000000
#define START 0
#define END 4000000

typedef struct
{
    long x;
    long y;
} Beacon;

typedef struct
{
    long x;
    long y;
    long d; // Range of sensor (Manhattan distance from sensor to beacon).
} Sensor;

// Return Manhattan distance between (x1,y1) and (x2,y2).
long m_dist(long x1, long y1, long x2, long y2)
{
    return labs(x1 - x2) + labs(y1 - y2);
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

// Sort sensors by x coordinate.
void bsort(Sensor **array, int n)
{
    Sensor temp;
    for (int step = 0; step < n - 1; step++) 
        for (int i = 0; i < n - step - 1; i++)
            if ((*array)[i].x > (*array)[i + 1].x)
            {
                temp.x = (*array)[i].x;
                temp.y = (*array)[i].y;
                temp.d = (*array)[i].d;
                
                (*array)[i].x = (*array)[i + 1].x;
                (*array)[i].y = (*array)[i + 1].y;
                (*array)[i].d = (*array)[i + 1].d;

                (*array)[i + 1].x = temp.x;
                (*array)[i + 1].y = temp.y;
                (*array)[i + 1].d = temp.d;
            }
}

// Put start and end coordinates of the coverage range across level y of the 
// sensor at (x0,y0) with reach d.
void get_coverage(long *start, long *end, long d, long x0, long y0, long y)
{
    *start = x0 - (d - labs(y0 - y));
    *end = x0 + (d - labs(y0 - y));
}

// Return number of lines in file. Get coordinates of sensors and beacons.
int parse_data(Sensor **sensors, Beacon **beacons) 
{
    FILE *f = fopen("input.txt", "r");
    char *line = NULL, *s1, *s2;
    size_t len;
    long bx, by; // Beacon coordinates.

    // Get number of lines in file (corresponds to no. of sensors and beacons).
    int n;
    for (n = 0; getline(&line, &len, f) != -1; n++);
    rewind(f);
    *sensors = malloc(n * sizeof(Sensor));
    *beacons = malloc(n * sizeof(Beacon));

    // Get coordinates for sensors and beacons.
    for (int i = 0; getline(&line, &len, f) != -1; i++)
    {
        replace(line, "Sensor at x=", "");
        replace(line, " closest beacon is at x=", "");
        replace(line, "y=", "");
        replace(line, ", ", ",");
        s1 = strtok(line, ":"); // Sensor coordinates.
        s2 = strtok(NULL, ":"); // Beacon coordinates.

        (*sensors)[i].x = strtol(strtok(s1, ","), NULL, 10);
        (*sensors)[i].y = strtol(strtok(NULL, ","), NULL, 10);

        (*beacons)[i].x = strtol(strtok(s2, ","), NULL, 10);
        (*beacons)[i].y = strtol(strtok(NULL, ","), NULL, 10);

        (*sensors)[i].d = m_dist((*sensors)[i].x, (*sensors)[i].y, 
            (*beacons)[i].x, (*beacons)[i].y);
    }
    fclose(f);
    free(line);
    return n;
}

// Return solution to part 1.
long part1(Sensor *sensors, Beacon *beacons, int n)
{
    long start, end, d;

    // Start and end coordinates of coverage by each sensor.
    long *s = malloc(n * sizeof(long));
    long *e = malloc(n * sizeof(long));
    for (int i = 0; i < n; i++)
    {
        d = m_dist(sensors[i].x, sensors[i].y, beacons[i].x, beacons[i].y);
        get_coverage(&start, &end, d, sensors[i].x, sensors[i].y, YLEVEL);
        s[i] = start;
        e[i] = end;
    }

    // Get minimum start point and maximum end point to calculate range.
    int min = 0, max = 0;
    for (int i = 0; i < n; i++)
    {
        if (s[i] < min)
            min = s[i];
        if (e[i] > max)
            max = e[i];
    }
    int range = abs(max - min);

    char *line = malloc((range + 1) * sizeof(char));
    for (int i = 0; i < range; i++)
        line[i] = '.';
    line[range] = 0;

    // Fill in line with coverage as determined by coords in s and e.
    for (int i = 0; i < n; i++)
        if (s[i] <= e[i])
            for (int j = s[i]; j <= e[i]; j++)
                line[j + abs(min)] = '#';

    // Count number of points in line where a beacon cannot be.
    int count = 0;
    for (int i = 0; line[i]; i++)
        count += line[i] == '#';

    free(line);
    free(s);
    free(e);

    // Reduce count by one for every beacon already on level.
    int *already_x = malloc(n * sizeof(int));
    int k = 0, new_x;
    for (int i = 0; i < n; i++)
    {
        new_x = 1;
        if (beacons[i].y == YLEVEL)
        {
            for (int j = 0; j < k; j++) // Check if already accounted for.
                if (already_x[j] == beacons[i].x)
                    new_x = 0;
            if (new_x)
            {
                count--;
                already_x[k++] = beacons[i].x;
            }
        }
    }
    free(already_x);
    return count;
}

// Return solution to part 2.
long long part2(Sensor **s, int n)
{
    bsort(s, n); // Sort sensors by x coordinate.

    for (long y = START; y <= END; y++)
        for (long x = START; x <= END; x++)
        {
            int found = 1;
            for (int i = 0; i < n; i++) // For sensor in (*sensors).
            {
                if (m_dist((*s)[i].x, (*s)[i].y, x, y) <= (*s)[i].d)
                {
                    x += (((*s)[i].x >= x) ? 1 : -1)*labs((*s)[i].x - x) 
                        + (*s)[i].d - labs((*s)[i].y - y);
                    found = 0;
                    break;
                }
            }
            if (found)
                return 4000000*x + y;
        }
}

int main()
{
    Sensor *sensors;
    Beacon *beacons;
    int n = parse_data(&sensors, &beacons);

    printf("1: %ld\n", part1(sensors, beacons, n));
    free(beacons);
    printf("2: %lld\n", part2(&sensors, n));
    free(sensors);
    return 0;
}