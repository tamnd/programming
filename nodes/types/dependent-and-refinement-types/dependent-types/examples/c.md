```c run id=variably-modified-types
#include <stdio.h>

/* The type of m depends on the run-time values rows and cols:
   it is "array of rows arrays of cols int", a variably modified type. */
static int trace(size_t n, int m[n][n]) {
    int sum = 0;
    for (size_t i = 0; i < n; i++)
        sum += m[i][i];
    return sum;
}

static void fill(size_t rows, size_t cols) {
    int m[rows][cols];
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            m[i][j] = (int)(i * cols + j);
    printf("int[%zu][%zu]: sizeof %zu bytes, row %zu bytes", rows, cols,
           sizeof m, sizeof m[0]);
    if (rows == cols)
        printf(", trace %d", trace(rows, m));
    printf("\n");
}

int main(void) {
    for (size_t n = 2; n <= 4; n++)
        fill(n, n);
    fill(2, 5);
    return 0;
}
```

```output for=variably-modified-types
int[2][2]: sizeof 16 bytes, row 8 bytes, trace 3
int[3][3]: sizeof 36 bytes, row 12 bytes, trace 12
int[4][4]: sizeof 64 bytes, row 16 bytes, trace 30
int[2][5]: sizeof 40 bytes, row 20 bytes
```

- Verdict: idiom. C99 has variably modified types: the type of a variable-length array, such as `int m[rows][cols]`, depends on values known only at run time, and `sizeof` and indexing use them. This is a form of dependent type, but C does not check it: nothing stops a call to `trace` with a wrong `n`. C has no refinement types; a struct with a checked constructor function is the usual idiom.
