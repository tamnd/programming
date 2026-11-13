```c run id=half-open-loop-and-struct
#include <stdbool.h>
#include <stdio.h>

// C has no range type. A struct with a start and an end (excluded) is a common idiom.
typedef struct {
    int start;
    int end;
} range;

static int range_len(range r) { return r.end > r.start ? r.end - r.start : 0; }
static bool range_has(range r, int x) { return r.start <= x && x < r.end; }

int main(void) {
    // The for loop with < is a half-open range.
    for (int i = 2; i < 7; i++) printf("%d ", i);
    printf("\n");

    range r = {2, 13};
    printf("%d %d %d %d\n", range_len(r), range_has(r, 2), range_has(r, 13), range_len((range){5, 5}));

    // Split a range at mid: the two halves are adjacent, with no gap and no overlap.
    int mid = r.start + range_len(r) / 2;
    range lo = {r.start, mid}, hi = {mid, r.end};
    printf("[%d,%d) [%d,%d) %d\n", lo.start, lo.end, hi.start, hi.end, range_len(lo) + range_len(hi));

    // A count down with an unsigned index: i-- > 0 stops after 0, so it does not wrap around.
    unsigned a[] = {10, 20, 30};
    for (size_t i = sizeof a / sizeof a[0]; i-- > 0;) printf("%u ", a[i]);
    printf("\n");
    return 0;
}
```

```output
2 3 4 5 6
11 1 0 0
[2,7) [7,13) 11
30 20 10
```

- Verdict: idiom. C has no range type. The for loop with < is a half-open range, and a struct with a start and an end gives the length and a membership test. A split at the middle gives two adjacent ranges. An unsigned count down uses i-- > 0, so it stops after 0.
