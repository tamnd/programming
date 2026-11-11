```c run id=sheet
#include <stdio.h>

enum { PRICE, QTY, SHIPPING, SUBTOTAL, TOTAL, N };
static double cell[N];
static int recomputed;

static double subtotal(void) { return cell[PRICE] * cell[QTY]; }
static double total(void) { return cell[SUBTOTAL] + cell[SHIPPING]; }

/* Formulas in topological order: each one reads only cells set before it. */
static const struct {
    int target;
    double (*fn)(void);
} formulas[] = { { SUBTOTAL, subtotal }, { TOTAL, total } };

static void set(int c, double v)
{
    cell[c] = v;
    for (size_t i = 0; i < sizeof formulas / sizeof formulas[0]; i++) {
        cell[formulas[i].target] = formulas[i].fn();
        recomputed++;
    }
}

int main(void)
{
    set(PRICE, 10);
    set(QTY, 3);
    set(SHIPPING, 5);
    printf("total %.0f\n", cell[TOTAL]);
    set(QTY, 4);
    printf("total %.0f after %d recomputations\n", cell[TOTAL], recomputed);
    return 0;
}
```

```output for=sheet
total 35
total 45 after 8 recomputations
```

- Verdict: absent. C has no events or closures to build observers from. A table of formulas in topological order makes a small spreadsheet: after each change, the program computes every formula once, in an order where each formula reads only values that are already up to date. This avoids glitches, but it recomputes all formulas, even those that the change does not affect.
