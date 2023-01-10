#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main()
{
    int n, v[25][25];
    scanf("%d", &n);
    srand(time(NULL));
    for (int i = 0; i < n; i++)
        for (int j = i; j < n; j++)
            if (i == j)
                v[i][j] = 0;
            else 
                v[i][j] = v[j][i] = rand() % 41 + 2;
    FILE *out = fopen("mat.out", "w");
    fprintf(out, "%d %d\n", n, rand() % n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            fprintf(out, "%d ", v[i][j]);
        fprintf(out, "\n");
    }
    fclose(out);
    return 0;
}