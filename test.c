#include <stdio.h>

int main()
{
    int v[100][100];
    int i, j, n;
    scanf("%d", &n);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%d", &v[i][j]);
    int s = 0;
    int currNode;
    scanf("%d", &currNode);
    for (i = 0; i < n; i++)
    {
        int nextNode;
        scanf("%d", &nextNode);
        s += v[currNode - 1][nextNode - 1];
        // printf("%d -> %d: %d si s = %d\n", currNode, nextNode, v[currNode][nextNode], s);
        currNode = nextNode;
    }
    printf("%d\n", s);
    return 0;
}