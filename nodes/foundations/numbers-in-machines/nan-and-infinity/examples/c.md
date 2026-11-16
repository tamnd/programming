```c run id=traps
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* A total order for qsort: a NaN comes after every number. */
static int cmp_double(const void *a, const void *b) {
    double x = *(const double *)a;
    double y = *(const double *)b;
    if (isnan(x) || isnan(y)) {
        return (isnan(x) != 0) - (isnan(y) != 0);
    }
    return (x > y) - (x < y);
}

int main(void) {
    double nan = NAN;
    double inf = INFINITY;
    printf("nan == nan: %s, isnan: %s\n", nan == nan ? "yes" : "no", isnan(nan) ? "yes" : "no");
    printf("inf - inf: %f, 1 / -inf: %f\n", inf - inf, 1.0 / -inf);
    printf("signbit(-0.0): %s, -0.0 == 0.0: %s\n", signbit(-0.0) ? "yes" : "no", -0.0 == 0.0 ? "yes" : "no");
    printf("fmax(1, NaN): %g\n", fmax(1.0, nan));

    double values[] = {3.0, nan, 1.0, 2.0, 0.5};
    qsort(values, 5, sizeof values[0], cmp_double);
    printf("qsort:");
    for (int i = 0; i < 5; i++) {
        printf(" %g", values[i]);
    }
    printf("\n");
    return 0;
}
```

```output for=traps
nan == nan: no, isnan: yes
inf - inf: -nan, 1 / -inf: -0.000000
signbit(-0.0): yes, -0.0 == 0.0: yes
fmax(1, NaN): 1
qsort: 0.5 1 2 3 nan
```

- Verdict: native. C `double` has `NAN`, `INFINITY` and the tests `isnan`, `isinf` and `signbit` in `<math.h>`.
- `qsort` needs a comparator that gives a total order. A comparator with only `<` and `>` returns 0 for every pair with a NaN, which says that the NaN equals each number. That is not a total order, and the result of the sort is then not defined. The comparator here tests for a NaN first and puts it last.
- `fmax` returns the number when the other argument is a NaN.
