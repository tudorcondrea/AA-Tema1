#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int *minimum_spanning_tree(unsigned int **v, unsigned int n, unsigned int startCity)
{
    unsigned int *weight, *visited;
    int *parent;
    unsigned int i, j, minWeight, minIndex = startCity;
    parent = malloc(sizeof(int) * n);
    weight = malloc(sizeof(int) * n);
    visited = malloc(sizeof(int) * n);
    for (i = 0; i < n; i++)
    {
        weight[i] = UINT_MAX;
        visited[i] = 0;
    }
    weight[startCity] = 0;
    parent[startCity] = -1;
    for (i = 0; i < n - 1; i++)
    {
        minWeight = UINT_MAX;
        for (j = 0; j < n; j++)
            if (visited[j] == 0 && weight[j] < minWeight)
            {
                minWeight = weight[j];
                minIndex = j;
            }
        visited[minIndex] = 1;
        for (j = 0; j < n; j++)
            if (v[minIndex][j] != 0 && visited[j] == 0 && v[minIndex][j] < weight[j])
            {
                parent[j] = minIndex;
                weight[j] = v[minIndex][j];
            }
    }
    free(weight);
    free(visited);
    return parent;
}

unsigned int *count_degrees(int *parent, unsigned int n)
{
    unsigned int *deg = malloc(sizeof(int) * n);
    unsigned int i;
    for (i = 0; i < n; i++)
        deg[i] = 0;
    for (i = 0; i < n; i++)
        if (parent[i] != -1)
        {   
            deg[parent[i]]++;
            deg[i]++;
        }
    return deg;
}

unsigned int form_odds(unsigned int *deg, unsigned int n, unsigned int *odds)
{
    unsigned int i, count = 0;
    for (i = 0; i < n; i++)
        if (deg[i] % 2 != 0)
            odds[count++] = i;
    return count;
}

unsigned int **adj_mat(unsigned int **v, unsigned int n, int *parent)
{
    unsigned int **adj = malloc(sizeof(int*) * n);
    unsigned int i, j;
    for (i = 0; i < n; i++)
        adj[i] = malloc(sizeof(int) * n);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            adj[i][j] = 0;
    for (i = 0; i < n; i++)
        if (parent[i] != -1)
            adj[i][parent[i]] = adj[parent[i]][i] = 1;
    return adj;
}

unsigned int **min_matching(unsigned int **v, unsigned int n, unsigned int *odds, unsigned int oddsCount, int *parent)
{
    unsigned int *visited = malloc(n * sizeof(int)), **adj = adj_mat(v, n, parent);
    unsigned int i, j, k, minCost, minIndexi = 0, minIndexj = 0;
    for (i = 0; i < n; i++)
        visited[i] = 1;
    for (i = 0; i < oddsCount; i++)
        visited[odds[i]] = 0;
    for (k = 0; k < oddsCount / 2; k++)
    {
        minCost = UINT_MAX;
        for (i = 0; i < n; i++)
            for (j = i + 1; j < n; j++)
                if (v[i][j] < minCost && visited[i] == 0 && visited[j] == 0)
                {
                    minCost = v[i][j];
                    minIndexi = i;
                    minIndexj = j;
                }
        visited[minIndexj] = visited[minIndexi] = 1;
        adj[minIndexi][minIndexj] = adj[minIndexj][minIndexi] += 1;
    }
    free(visited);
    return adj;
}

unsigned int count_edges(unsigned int **v, unsigned int n)
{
    unsigned int i, j, count = 0;
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            count += v[i][j];
    return count;
}

unsigned int count_neighbours(unsigned int **v, unsigned int n, unsigned int startCity)
{
    unsigned int i, count = 0;
    for (i = 0; i < n; i++)
        if (v[i][startCity] != UINT_MAX && i != startCity)
            count += v[i][startCity];
    return count;
}

unsigned int *euler_tour(unsigned int **v, unsigned int n, unsigned int **adj, unsigned int edgeCount, unsigned int startCity)
{
    unsigned int i;
    unsigned int *partTour, *compTour, partCount = 0, compCount = 0;
    partTour = malloc(sizeof(int) * edgeCount);
    compTour = malloc(sizeof(int) * edgeCount);
    partTour[partCount++] = startCity;
    while (partCount > 0)
    {
        if (count_neighbours(adj, n, partTour[partCount - 1]) == 0)
        {
            compTour[compCount++] = partTour[partCount - 1];
            partCount--;
        }
        else
        {
            for (i = 0; i < n; i++)
                if (v[i][partTour[partCount - 1]] != UINT_MAX && i != partTour[partCount - 1] && adj[i][partTour[partCount - 1]] != 0)
                {
                    adj[i][partTour[partCount - 1]] = adj[partTour[partCount - 1]][i] -= 1;
                    if (adj[i][partTour[partCount - 1]] == 0)
                        v[i][partTour[partCount - 1]] = v[partTour[partCount - 1]][i] = UINT_MAX;
                    partTour[partCount++] = i;
                    break;
                }
        }
    }
    free(partTour);
    return compTour;
}

unsigned int hamiltonize(unsigned int **v, unsigned int n, unsigned int *compTour, unsigned int *edgeCount)
{
    unsigned int *visited = malloc(sizeof(int) * n), pathCost = 0;
    unsigned int i, j;
    for (i = 0; i < n; i++)
        visited[i] = 0;
    visited[compTour[0]] = 1;
    i = 1;
    while (i < *edgeCount - 1)
    {
        if (visited[compTour[i]] == 0)
        {
            pathCost += v[compTour[i]][compTour[i - 1]];
            visited[compTour[i]] = 1;
            i++;
        }
        else
        {
            for (j = i; j < *edgeCount - 1; j++)
                compTour[j] = compTour[j + 1];
            *edgeCount -= 1;
        }
    }
    pathCost += v[compTour[*edgeCount - 1]][compTour[*edgeCount - 2]];
    free(visited);
    return pathCost;
}

int main(void)
{
    unsigned int **v, n, startCity;
    unsigned int i, j;

    FILE *in = fopen("test.in", "r");
    if (fscanf(in, "%d%d", &n, &startCity) == 0)
        return -1;
    v = malloc(sizeof(int*) * n);
    for (i = 0; i < n; i++)
        v[i] = malloc(sizeof(int) * n);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (fscanf(in, "%d", &v[i][j]) == 0)
                return -1;
    
    int *parent = minimum_spanning_tree(v, n, startCity);
    unsigned int *deg = count_degrees(parent, n);
    unsigned int *odds = malloc (sizeof(int) * n), oddsCount;
    oddsCount = form_odds(deg, n, odds);
    unsigned int **adj = min_matching(v, n, odds, oddsCount, parent);
    unsigned int edgeCount = count_edges(adj, n);
    edgeCount++;
    unsigned int **vCopy = malloc(sizeof(int*) * n);
    for (i = 0; i < n; i++)
    {
        vCopy[i] = malloc(sizeof(int) * n);
        for (j = 0; j < n; j++)
            vCopy[i][j] = v[i][j]; 
    }
    unsigned int *etour = euler_tour(vCopy, n, adj, edgeCount, startCity);
    for (i = 0; i < edgeCount / 2; i++) 
    {
        unsigned int aux = etour[i];
        etour[i] = etour[edgeCount - 1 - i];
        etour[edgeCount - 1 - i] = aux;
    }
    unsigned int pathCost = hamiltonize(v, n, etour,  &edgeCount);

    FILE *out = fopen("test.out", "w");
    if (fprintf(out, "%d\n", pathCost) == 0)
        return -1;
    for (i = 0; i < n + 1; i++)
        if (fprintf(out, "%d ", etour[i]) == 0)
            return -1;
    fprintf(out, "\n");
    for (i = 0; i < n; i++)
    {
        free(v[i]);
        free(adj[i]);
        free(vCopy[i]);
    }
    free(v);
    free(vCopy);
    free(parent);
    free(deg);
    free(odds);
    free(adj);
    free(etour);
    fclose(in);
    fclose(out);
    return 0;
}
