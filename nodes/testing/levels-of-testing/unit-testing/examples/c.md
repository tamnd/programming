```c run id=cart
#include <assert.h>
#include <stdio.h>

struct item {
    int price;
    int quantity;
};

/* Writes the total to *total and returns 0, or returns -1 for a negative quantity. */
static int cart_total(const struct item *items, int count, int *total) {
    int sum = 0;
    for (int i = 0; i < count; i++) {
        if (items[i].quantity < 0) {
            return -1;
        }
        sum += items[i].price * items[i].quantity;
    }
    *total = sum;
    return 0;
}

static void test_empty_cart_is_zero(void) {
    int total = -1;
    assert(cart_total(NULL, 0, &total) == 0);
    assert(total == 0);
}

static void test_sums_price_times_quantity(void) {
    const struct item items[] = {{250, 2}, {100, 1}};
    int total = 0;
    assert(cart_total(items, 2, &total) == 0);
    assert(total == 600);
}

static void test_rejects_negative_quantity(void) {
    const struct item items[] = {{250, -1}};
    int total = 0;
    assert(cart_total(items, 1, &total) == -1);
}

int main(void) {
    test_empty_cart_is_zero();
    test_sums_price_times_quantity();
    test_rejects_negative_quantity();
    puts("3 tests passed");
    return 0;
}
```

```output for=cart
3 tests passed
```

- Verdict: idiom. ISO C has no test framework. The usual minimal form is a test program: one function for each test, `assert` for each check, and a `main` that calls the tests.
- `assert` prints the failed expression, the file and the line, and then calls `abort`. So the first failure stops the whole program, and the tests after it do not run.
- The macro `NDEBUG` turns every `assert` into nothing. A test build must not define it.
- Libraries such as Unity, CMocka and Check add test registration, a report of each test, and checks that go on after a failure.
