```c run id=orders
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct order { char customer[8]; int amount; };     /* a value type */

/* An interface as a table of function pointers. */
struct report { const char *fmt; };
static void line(const struct report *r, const char *n, int t) { printf(r->fmt, n, t); }

struct total { char name[8]; int sum; };

static int by_sum_desc(const void *a, const void *b) {   /* a function as a value */
    return ((const struct total *)b)->sum - ((const struct total *)a)->sum;
}

int main(void) {
    struct order orders[] = {{"ana", 30}, {"bo", 70}, {"ana", 25}, {"cy", 10}, {"bo", 5}};
    struct total totals[5];
    int n = 0;
    for (size_t i = 0; i < sizeof orders / sizeof orders[0]; i++) {   /* procedural */
        int j = 0;
        while (j < n && strcmp(totals[j].name, orders[i].customer) != 0) j++;
        if (j == n) { strcpy(totals[n].name, orders[i].customer); totals[n++].sum = 0; }
        totals[j].sum += orders[i].amount;
    }
    qsort(totals, n, sizeof totals[0], by_sum_desc);
    const struct report reports[] = {{"%s %d\n"}, {"%s,%d\n"}};
    for (int r = 0; r < 2; r++)
        for (int j = 0; j < n; j++)
            if (totals[j].sum > 50) line(&reports[r], totals[j].name, totals[j].sum);
    return 0;
}
```

```output for=orders
bo 75
ana 55
bo,75
ana,55
```

- Verdict: idiom. C is a procedural language. Function pointers give functions as values, as the comparator of `qsort` shows, and a struct of function pointers gives an interface, which is how the Linux kernel and GTK write object-oriented code. Here the two formats differ only in data, so a format string is enough. C has no closures, so a callback takes a pointer to its context as an extra argument.
