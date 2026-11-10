```c run id=checker
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* xorshift32 with Marsaglia's seed: the same numbers on every run. */
static uint32_t state = 2463534242u;

static uint32_t next(void) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

static int64_t between(int64_t lo, int64_t hi) {
    return lo + (int64_t)(next() % (uint64_t)(hi - lo + 1));
}

/* Property 1, a round trip: parsing the decimal form gives the number back. */
static bool round_trip(int32_t x) {
    char text[16];
    snprintf(text, sizeof text, "%" PRId32, x);
    return strtol(text, NULL, 10) == x;
}

/* Property 2, an invariant: the pages hold all the items. */
static int pages(int items, int per_page) {
    return items / per_page; /* bug: drops the last page when it is not full */
}

static bool pages_hold_items(int items, int per_page) {
    return pages(items, per_page) * per_page >= items;
}

/* Candidates from the smallest up: lo, then x - d for d = (x - lo) / 2, / 4, ..., 1. */
static int shrink_int(int x, int lo, int out[]) {
    int n = 0;
    if (x > lo) {
        out[n++] = lo;
    }
    for (int d = (x - lo) / 2; d > 0; d /= 2) {
        out[n++] = x - d;
    }
    return n;
}

int main(void) {
    state = 2463534242u;
    bool passed = true;
    for (int trial = 1; trial <= 100 && passed; trial++) {
        int32_t x = (int32_t)between(INT32_MIN, INT32_MAX);
        if (!round_trip(x)) {
            printf("round trip: falsified on test %d by %" PRId32 "\n", trial, x);
            passed = false;
        }
    }
    if (passed) {
        printf("round trip: passed 100 tests\n");
    }

    state = 2463534242u;
    for (int trial = 1; trial <= 100; trial++) {
        int items = (int)between(0, 1000);
        int per_page = (int)between(1, 50);
        if (pages_hold_items(items, per_page)) {
            continue;
        }
        printf("pages: falsified on test %d by (%d, %d)\n", trial, items, per_page);
        int steps = 0;
        bool shrunk = true;
        while (shrunk) {
            shrunk = false;
            int candidates[40];
            int n = shrink_int(items, 0, candidates);
            for (int i = 0; i < n && !shrunk; i++) {
                if (!pages_hold_items(candidates[i], per_page)) {
                    items = candidates[i];
                    shrunk = true;
                }
            }
            n = shrink_int(per_page, 1, candidates);
            for (int i = 0; i < n && !shrunk; i++) {
                if (!pages_hold_items(items, candidates[i])) {
                    per_page = candidates[i];
                    shrunk = true;
                }
            }
            steps += shrunk;
        }
        printf("pages: shrunk in %d steps to (%d, %d)\n", steps, items, per_page);
        return 0;
    }
    printf("pages: passed 100 tests\n");
    return 0;
}
```

```output
round trip: passed 100 tests
pages: falsified on test 1 by (967, 7)
pages: shrunk in 13 steps to (1, 2)
```

The first fix rounds up with `(items + per_page - 1) / per_page`. In `uint32_t`, the sum wraps near the top of the range, and the program goes on with a wrong count. The generator takes a value near `UINT32_MAX` on one test in four, because a uniform choice almost never comes close.

```c wrong id=wrap
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t state = 2463534242u;

static uint32_t next(void) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

/* One test in four takes a value near the maximum; a uniform choice almost never does. */
static uint32_t edgy_u32(void) {
    if (next() % 4 == 0) {
        return UINT32_MAX - next() % 4;
    }
    return next();
}

/* The fix for the last page: the rounded-up quotient. */
static uint32_t pages(uint32_t items, uint32_t per_page) {
    return (items + per_page - 1) / per_page; /* wraps when items is near UINT32_MAX */
}

static bool pages_hold_items(uint32_t items, uint32_t per_page) {
    return (uint64_t)pages(items, per_page) * per_page >= items;
}

/* Candidates from the smallest up: lo, then x - d for d = (x - lo) / 2, / 4, ..., 1. */
static int shrink_u32(uint32_t x, uint32_t lo, uint32_t out[]) {
    int n = 0;
    if (x > lo) {
        out[n++] = lo;
    }
    for (uint32_t d = (x - lo) / 2; d > 0; d /= 2) {
        out[n++] = x - d;
    }
    return n;
}

int main(void) {
    for (int trial = 1; trial <= 100; trial++) {
        uint32_t items = edgy_u32();
        uint32_t per_page = 1 + next() % 50;
        if (pages_hold_items(items, per_page)) {
            continue;
        }
        printf("falsified on test %d by (%u, %u)\n", trial, items, per_page);
        int steps = 0;
        bool shrunk = true;
        while (shrunk) {
            shrunk = false;
            uint32_t candidates[40];
            int n = shrink_u32(items, 0, candidates);
            for (int i = 0; i < n && !shrunk; i++) {
                if (!pages_hold_items(candidates[i], per_page)) {
                    items = candidates[i];
                    shrunk = true;
                }
            }
            n = shrink_u32(per_page, 1, candidates);
            for (int i = 0; i < n && !shrunk; i++) {
                if (!pages_hold_items(items, candidates[i])) {
                    per_page = candidates[i];
                    shrunk = true;
                }
            }
            steps += shrunk;
        }
        printf("shrunk in %d steps to (%u, %u): pages = %u\n", steps, items, per_page, pages(items, per_page));
        return 0;
    }
    printf("passed 100 tests\n");
    return 0;
}
```

```output for=wrap
falsified on test 6 by (4294967294, 5)
shrunk in 2 steps to (4294967292, 5): pages = 0
```

- Verdict: idiom. The C standard library has no property checker. The example writes one as a loop over a seeded generator.
- C has no closures and no generic functions. The checker for pairs is written out by hand, and a general checker would take function pointers and `void *` cases.
- `uint32_t` arithmetic wraps by rule, so xorshift32 needs no masks. The same wrap hides the overflow in the `wrong` block: `pages` returns 0 for 4294967292 items, and no error occurs. Rust panics on the same code in a debug build.
- Signed overflow in C is undefined behavior, so `pages(items, per_page) * per_page` must stay small. The generator keeps `items` under 1001 for this reason.
