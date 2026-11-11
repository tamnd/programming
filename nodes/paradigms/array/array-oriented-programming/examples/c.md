```c run id=loops
#include <stdio.h>

#define N 5

int main(void) {
    int a[N] = {3, 1, 4, 1, 5};
    int b[N] = {2, 7, 1, 8, 2};
    int sum[N], dot = 0, scan[N], run = 0;

    for (int i = 0; i < N; i++) {
        sum[i] = a[i] + b[i]; /* element-wise */
        dot += a[i] * b[i];   /* reduction */
        run += a[i];
        scan[i] = run;        /* running total */
    }
    for (int i = 0; i < N; i++) printf("%d ", sum[i]);
    printf("| %d |", dot);
    for (int i = 0; i < N; i++) printf(" %d", scan[i]);
    printf("\n");
    return 0;
}
```

```output for=loops
5 8 5 9 7 | 35 | 3 4 8 9 14
```

- Verdict: absent. C has no operations on whole arrays: an array name in an expression becomes a pointer to the first element, and every operation is a loop over the indices. Compilers vectorize simple loops of this kind, and GCC and Clang have vector types as an extension. BLAS and LAPACK are the libraries that give array operations to C and Fortran programs.
