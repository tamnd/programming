```c run id=declarators-and-subscripts
#include <stdio.h>

typedef int T;

int main(void) {
    /* a[i] is defined as *(a + i), so i[a] is also valid. */
    int a[3] = {10, 20, 30};
    printf("%d %d %d\n", a[1], *(a + 1), 1[a]);

    /* The declarator syntax reads from the name outward. */
    int *p[3];      /* an array of 3 pointers to int */
    int (*q)[3];    /* a pointer to an array of 3 ints */
    q = &a;
    p[0] = &a[2];
    printf("%zu %zu %d %d\n", sizeof p / sizeof p[0], sizeof *q / sizeof (*q)[0], *p[0], (*q)[0]);

    /* The parse of T * x depends on whether T names a type. */
    T * x = &a[0];  /* a declaration, because T is a typedef */
    printf("%d\n", *x);

    /* x+++y is read as x++ + y: the lexer takes the longest token. */
    int i = 1, j = 2;
    int k = i+++j;
    printf("k = %d, i = %d\n", k, i);
    return 0;
}
```

```output for=declarators-and-subscripts
20 20 20
3 3 30 10
10
k = 3, i = 2
```

- Verdict: native. `a[1]` is defined as `*(a + 1)`, so `1[a]` is also valid. `int *p[3]` is an array of pointers, and `int (*q)[3]` is a pointer to an array. `T * x` is a declaration because `T` is a typedef. The lexer takes the longest token, so `i+++j` is `i++ + j`.
