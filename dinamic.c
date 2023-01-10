#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h> 
 
unsigned int count_set_bits(unsigned int n) 
{ 
    unsigned int count = 0; 
    while (n) 
    { 
        n &= n-1; 
        count++; 
    } 
    return count; 
} 
 
unsigned int choose(unsigned int n, unsigned int k) 
{ 
    if (k == 0) 
        return 1; 
    return (n * choose(n - 1, k - 1)) / k; 
} 
 
/* 
 * generates all numbers that have exactly k set bits out of a maximum n 
 */ 
unsigned int *gen_bit_combinations(unsigned int k, unsigned int n, int size) 
{ 
    unsigned int *set = malloc(sizeof(int) * size); 
    unsigned int subset = 1, count = 0; 
    while (subset < (unsigned int)(1 << n)) 
    { 
        if (count_set_bits(subset) == k) 
        { 
            set[count] = subset; 
            count++; 
        } 
        subset++; 
    } 
    return set; 
} 
 
/* 
 * solves the problem, returns the optimal path length and modifies tour to remember the path 
 */ 
unsigned int tsp(unsigned int n, unsigned int **v, unsigned int startCity, unsigned int *tour) 
{ 
    // edge case for only 2 cities 
    if (n == 2) 
    { 
        tour = malloc(sizeof(int) * 3); 
        tour[0] = tour[2] = startCity; 
        if (startCity == 0) 
            tour[1] = 1; 
        else 
            tour[1] = 0; 
        return v[0][1]; 
    } 
    unsigned int i, j, k, pivot, partSubset, minDist, currDist, endNode; 
    unsigned int **partTour = malloc(sizeof(int*) * n); 
    // partTour remember all partial tours that contain subsets of cities, 
    // ending in a fixed city, with their respective length 
    for (i = 0; i < n; i++) 
        partTour[i] = malloc(sizeof(int) * (1 << n)); 
    for (i = 0; i < n; i++) 
        for (j = 0; j < (unsigned int)(1 << n); j++) 
            partTour[i][j] = UINT_MAX; 
    // start with the initial city and it's adjacent cities 
    // to form lengths 3 then 4 then so on 
    for (i = 0; i < n; i++) 
        if (i != startCity) 
            partTour[i][1 << startCity | 1 << i] = v[startCity][i]; 
    // a bit mask is used where the ith set bit means that 
    // city number i is part of the current set 
    for (k = 3; k <= n; k++) 
    { 
        unsigned int cmbNK = choose(n, k); 
        unsigned int *set = gen_bit_combinations(k, n, cmbNK); 
        // for the set to be valid, it needs to contain the starting city 
        for (j = 0; j < cmbNK; j++) 
            if (((1 << startCity) & set[j]) != 0) 
                for (pivot = 0; pivot < n; pivot++) 
                    if (((1 << pivot) & set[j]) != 0 && pivot != startCity) 
                    { 
                        // remove 1 city a time to see how long the path is to that city 
                        partSubset = set[j] ^ (1 << pivot); 
                        minDist = UINT_MAX; 
                        for (endNode = 0; endNode < n; endNode++) 
                            if (endNode != startCity && endNode != pivot && ((1 << endNode) & partSubset) != 0) 
                            { 
                                currDist = partTour[endNode][partSubset] + v[endNode][pivot]; 
                                if (currDist < minDist) 
                                { 
                                    minDist = currDist; 
                                } 
                            } 
                        partTour[pivot][set[j]] = minDist; 
                    } 
        free(set); 
    } 
    // we take all partial tours that cover all cities and close them off to see which is best 
    minDist = UINT_MAX; 
    for (endNode = 0; endNode < n; endNode++) 
        if (endNode != startCity) 
        { 
            currDist = partTour[endNode][(1 << n) - 1] + v[endNode][startCity]; 
            if (currDist < minDist) 
                minDist = currDist; 
        } 
     
    // reconstruct the path by breaking it down and seeing 
    // which is the best subset leading to each city 
    unsigned int prevDist, newDist, state = (1 << n) - 1, lastIndex = startCity; 
    for (i = n - 1; i >= 1; i--) 
    { 
        int index = -1; 
        prevDist = UINT_MAX; 
        for (j = 0; j < n; j++) 
            if (j != startCity && ((1 << j) & state) != 0) 
            { 
                newDist = partTour[j][state] + v[j][lastIndex]; 
                if (newDist < prevDist) 
                { 
                    index = j; 
                    prevDist = newDist; 
                } 
                 
            } 
        tour[i] = index; 
        state ^= (1 << index); 
        lastIndex = index; 
    } 
    tour[0] = tour[n] = startCity; 
    for (i = 0; i < n; i++) 
        free(partTour[i]); 
    free(partTour); 
    return minDist; 
} 
 
int main(void) 
{ 
    unsigned int n, **v, *tour; 
    unsigned int i, j, startCity; 
 
    FILE *in = fopen("test.in", "r"); 
    if (fscanf(in, "%d%d", &n, &startCity) == 0) 
        return -1; 
    v = malloc(sizeof(int*) * n); 
    tour = malloc(sizeof(int) * (n + 1)); 
    for (i = 0; i < n; i++) 
        v[i] = malloc(sizeof(int) * n); 
    for (i = 0; i < n; i++) 
        for (j = 0; j < n; j++) 
            if (fscanf(in, "%d", &v[i][j]) == 0) 
                return -1; 
     
    FILE *out = fopen("test.out", "w"); 
    fprintf(out, "%d\n", tsp(n, v, startCity, tour)); 
    for (i = n; (int)i >= 0; i--) 
        fprintf(out, "%d ", tour[i]); 
    fprintf(out, "\n"); 
 
    for (i = 0; i < n; i++) 
        free(v[i]); 
    free(v); 
    free(tour); 
    fclose(in); 
    fclose(out); 
    return 0; 
} 
