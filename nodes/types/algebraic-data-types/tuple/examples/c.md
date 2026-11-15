```c run id=pair
#include <stdio.h>

struct min_max {
    int lo;
    int hi;
};

static struct min_max find_min_max(const int *values, int n) {
    struct min_max r = {values[0], values[0]};
    for (int i = 1; i < n; i++) {
        if (values[i] < r.lo) {
            r.lo = values[i];
        }
        if (values[i] > r.hi) {
            r.hi = values[i];
        }
    }
    return r;
}

int main(void) {
    int values[] = {7, 2, 9, 4};
    struct min_max pair = find_min_max(values, 4);
    printf("pair: (%d, %d)\n", pair.lo, pair.hi);

    int lo = pair.lo, hi = pair.hi;
    printf("lo: %d hi: %d\n", lo, hi);
    printf("second part: %d\n", pair.hi);

    struct min_max other = {2, 9};
    int equal = pair.lo == other.lo && pair.hi == other.hi;
    printf("equal pairs: %s\n", equal ? "yes" : "no");
    return 0;
}
```

```output for=pair
pair: (2, 9)
lo: 2 hi: 9
second part: 9
equal pairs: yes
```

- Verdict: idiom. C has no tuple type. A small `struct` returns several values from a function.
- The initializer `{values[0], values[0]}` fills the members by position.
- C has no `==` for structs, so the program compares the members.
