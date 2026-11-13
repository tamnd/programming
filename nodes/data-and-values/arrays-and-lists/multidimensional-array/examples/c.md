```c run id=row-major-block
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // int a[2][3] is an array of 2 arrays of 3 ints, stored in row-major order.
    int a[2][3] = {{1, 2, 3}, {4, 5, 6}};
    printf("%zu %zu %td\n", sizeof a, sizeof a[0], &a[1][0] - &a[0][0]);

    // The items are in one block, so a pointer to the first int walks all six in order.
    const int *p = &a[0][0];
    for (int i = 0; i < 6; i++) printf("%d ", p[i]);
    printf("\n");

    // A pointer to a row has the type int (*)[3].
    int (*row)[3] = &a[1];
    printf("%d %d\n", (*row)[2], a[1][2]);

    // A jagged array is an array of pointers to rows of different lengths.
    int r0[] = {1}, r1[] = {2, 3}, r2[] = {4, 5, 6};
    int *jag[] = {r0, r1, r2};
    int len[] = {1, 2, 3};
    for (int r = 0; r < 3; r++) printf("%d:%d ", len[r], jag[r][len[r] - 1]);
    printf("\n");

    // A matrix with sizes known only at run time: one malloc and an index formula.
    int rows = 2, cols = 3;
    int *m = malloc(sizeof *m * (size_t)rows * (size_t)cols);
    if (!m) return 1;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) m[i * cols + j] = 10 * i + j;
    printf("%d %d\n", m[1 * cols + 2], m[rows * cols - 1]);
    free(m);
    return 0;
}
```

```output
24 12 3
1 2 3 4 5 6
6 6
1:1 2:3 3:6
12 12
```

- Verdict: native. int a[2][3] is 24 bytes with rows of 12 bytes, and a[1][0] is 3 items after a[0][0]. One pointer walks all six items in row-major order. A pointer to a row has the type int (*)[3]. An array of pointers to rows of different lengths is a jagged array. One malloc and an index formula make a matrix with sizes known at run time.
