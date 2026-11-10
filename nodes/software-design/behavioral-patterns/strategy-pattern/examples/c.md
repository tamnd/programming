```c run id=checkout
#include <stddef.h>
#include <stdio.h>

struct item {
    const char *name;
    int price; /* in cents */
    int quantity;
};

typedef int (*pricing_fn)(const struct item *items, size_t count);

struct checkout {
    pricing_fn pricing;
};

static int checkout_total(const struct checkout *c, const struct item *items, size_t count) {
    return c->pricing(items, count);
}

static int regular(const struct item *items, size_t count) {
    int total = 0;
    for (size_t i = 0; i < count; i++) {
        total += items[i].price * items[i].quantity;
    }
    return total;
}

static int ten_percent_off(const struct item *items, size_t count) {
    return regular(items, count) * 90 / 100;
}

/* 50 cents off each unit of a line with 5 or more units. */
static int bulk(const struct item *items, size_t count) {
    int total = 0;
    for (size_t i = 0; i < count; i++) {
        int price = items[i].quantity >= 5 ? items[i].price - 50 : items[i].price;
        total += price * items[i].quantity;
    }
    return total;
}

int main(void) {
    const struct item cart[] = {{"book", 1500, 2}, {"pen", 200, 5}};
    const size_t count = sizeof cart / sizeof cart[0];
    const struct {
        const char *name;
        pricing_fn pricing;
    } strategies[] = {
        {"regular", regular},
        {"ten percent off", ten_percent_off},
        {"bulk", bulk},
    };
    struct checkout checkout = {regular};
    for (size_t i = 0; i < sizeof strategies / sizeof strategies[0]; i++) {
        checkout.pricing = strategies[i].pricing;
        printf("%s: %d cents\n", strategies[i].name, checkout_total(&checkout, cart, count));
    }
    return 0;
}
```

```output
regular: 4000 cents
ten percent off: 3600 cents
bulk: 3750 cents
```

- Verdict: idiom. A function pointer in a struct is the strategy. `qsort` in the standard library takes its comparison as a function pointer in the same way.
- C has no closures. So a strategy that needs configuration, such as the percent of a discount, needs a `void *` context argument next to the pointer.
- The struct of function pointers is the class form. It is how C code builds a vtable by hand, for a strategy with several operations.
