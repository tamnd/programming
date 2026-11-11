```c run id=fixpoint
#include <stdbool.h>
#include <stdio.h>

enum { ZEUS, ARES, HARMONIA, SEMELE, DIONYSUS, N };
static const char *name[N] = { "zeus", "ares", "harmonia", "semele", "dionysus" };

int main(void)
{
    bool parent[N][N] = { 0 }, ancestor[N][N] = { 0 };
    parent[ZEUS][ARES] = parent[ARES][HARMONIA] = true;
    parent[HARMONIA][SEMELE] = parent[SEMELE][DIONYSUS] = true;

    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++)
            ancestor[x][y] = parent[x][y];

    for (bool changed = true; changed;) {
        changed = false;
        for (int x = 0; x < N; x++)
            for (int z = 0; z < N; z++)
                for (int y = 0; y < N; y++)
                    if (parent[x][z] && ancestor[z][y] && !ancestor[x][y])
                        ancestor[x][y] = changed = true;
    }

    int count = 0;
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++)
            count += ancestor[x][y];
    printf("%d\n", count);
    for (int y = 0; y < N; y++)
        if (ancestor[ZEUS][y])
            printf("%s ", name[y]);
    printf("\n");
    return 0;
}
```

```output for=fixpoint
10
ares harmonia semele dionysus 
```

- Verdict: absent. C has no logic engine. With a small, fixed set of constants, a relation fits in a matrix of Booleans, and the rule becomes a triple loop.
