#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
    int n, dummy;
    double *x, *y;
    FILE *in = fopen("coord.in", "r");
    fscanf(in, "%d", &n);
    x = malloc(sizeof(double) * n);
    y = malloc(sizeof(double) * n);
    for (int i = 0; i < n; i++)
    {
        fscanf(in, "%d %lf %lf\n", &dummy, &x[i], &y[i]);
    }
    FILE *out = fopen("mat.out", "w");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            fprintf(out, "%d ", (int)ceil(sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]))));
        fprintf(out, "\n");
    }
    fclose(in);
    fclose(out);
    free(x);
    free(y);
    return 0;
}
