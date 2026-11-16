```c run id=graph
#include <stdbool.h>
#include <stdio.h>

#define N 4

static const char names[N] = {'a', 'b', 'c', 'd'};

int main(void) {
    int edges[][2] = {{0, 1}, {0, 2}, {1, 2}, {2, 3}, {3, 0}};
    int edge_count = (int)(sizeof edges / sizeof edges[0]);

    bool adjacent[N][N] = {{false}};
    for (int i = 0; i < edge_count; i++) {
        adjacent[edges[i][0]][edges[i][1]] = true;
    }

    for (int x = 0; x < N; x++) {
        printf("%c ->", names[x]);
        for (int y = 0; y < N; y++) {
            if (adjacent[x][y]) {
                printf(" %c", names[y]);
            }
        }
        printf("\n");
    }

    printf("edge a->c: %s\n", adjacent[0][2] ? "yes" : "no");
    printf("edge c->a: %s\n", adjacent[2][0] ? "yes" : "no");

    int in_degree = 0;
    int count = 0;
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            if (adjacent[x][y]) {
                count++;
                if (y == 2) {
                    in_degree++;
                }
            }
        }
    }
    printf("in-degree of c: %d\n", in_degree);
    printf("edges: %d\n", count);
    return 0;
}
```

```output for=graph
a -> b c
b -> c
c -> d
d -> a
edge a->c: yes
edge c->a: no
in-degree of c: 2
edges: 5
```

- Verdict: idiom. The nodes are numbered 0 to 3, and a two-dimensional `bool` array is the adjacency matrix.
- The edge test reads one cell. Listing the neighbors of a node reads a whole row.
