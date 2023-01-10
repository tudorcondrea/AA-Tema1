#include <stdio.h>

void bkt(unsigned int n, unsigned int v[20][20], unsigned int currCity, unsigned int startCity,
         unsigned int visited[20], unsigned int queue[20], unsigned int best[20],
         unsigned int currDist, unsigned int *bestDist, unsigned int level)
{
    // found a possible solution, verify it
    if (level == n - 1)
    {
        // tour is incomplete, misses closing vertex
        if (*bestDist > currDist + v[startCity][queue[n - 2]])
        {
            for (unsigned int i = 0; i < n; i++)
                best[i] = queue[i];
            *bestDist = currDist + v[startCity][queue[n - 2]];
        }
    }
    else
    {
        for (unsigned int i = 0; i < n; i++)
            if (visited[i] == 0 && i != currCity)
            {
                visited[i] = 1;
                queue[level] = i;
                bkt(n, v, i, startCity, visited, queue, best, currDist + v[currCity][i], bestDist, level + 1);
                visited[i] = 0;
            }
    }
}

int main(void)
{
    unsigned int n, v[20][20], queue[20], best[20], bestDist = 2 << 30;
    unsigned int i, j;
    unsigned int startCity, visited[20];

    FILE *in = fopen("test.in", "r");
    if (fscanf(in, "%d%d", &n, &startCity) == 0)
        return -1;
    for (i = 0; i < n; i++)
        visited[i] = 0;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            v[i][j] = 0;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (fscanf(in, "%d", &v[i][j]) == 0)
                return -1;

    visited[startCity] = 1;
    queue[n - 1] = startCity;
    bkt(n, v, startCity, startCity, visited, queue, best, 0, &bestDist, 0);

    FILE *out = fopen("test.out", "w");
    if (fprintf(out, "%d\n%d ", bestDist, startCity) == 0)
        return -1;
    for (i = 0; i < n; i++)
        if (fprintf(out, "%d ", best[i]) == 0)
            return -1;
    fprintf(out, "\n");
    fclose(in);
    fclose(out);
    return 0;
}
