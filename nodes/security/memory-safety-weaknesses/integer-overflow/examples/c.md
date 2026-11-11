```c fails expect=sanitizer:undefined timeout=30 id=multiply
#include <stdio.h>

// Returns 1 if the order costs no more than the budget.
int order_fits(int price, int quantity, int budget) {
    int total = price * quantity; // can overflow
    return total <= budget;
}

int main(void) {
    printf("3 items: %d\n", order_fits(100, 3, 1000));
    printf("42949673 items: %d\n", order_fits(100, 42949673, 1000));
}
```

UndefinedBehaviorSanitizer reports the signed overflow in `order_fits`. Without the sanitizer, the behavior is undefined. A build that wraps the product gets 4, and the large order fits in the budget.

```c run flags="-std=c23 -fsanitize=address,undefined" id=ckd-mul
#include <stdckdint.h>
#include <stdio.h>

// Returns 1 if the order costs no more than the budget.
int order_fits(int price, int quantity, int budget) {
    int total;
    if (price < 0 || quantity < 0) {
        return 0;
    }
    if (ckd_mul(&total, price, quantity)) {
        return 0; // the total does not fit in an int
    }
    return total <= budget;
}

int main(void) {
    printf("3 items: %d\n", order_fits(100, 3, 1000));
    printf("42949673 items: %d\n", order_fits(100, 42949673, 1000));
}
```

```output for=ckd-mul
3 items: 1
42949673 items: 0
```

```c wrong id=unsigned-wrap
#include <stdio.h>

int main(void) {
    unsigned int stock = 2;
    unsigned int sold = 3;
    unsigned int left = stock - sold; // wraps to 4294967295
    printf("left: %u\n", left);
}
```

```output
left: 4294967295
```

Unsigned arithmetic wraps by definition, so the program has no undefined behavior, and the default set of UndefinedBehaviorSanitizer checks does not report it. Clang has the extra check `-fsanitize=unsigned-integer-overflow` for it. GCC has no such check. This is an underflow, CWE-191: 2 − 3 is not −1 but 4,294,967,295.

- Verdict: absent. C has no overflow check in its arithmetic. Signed overflow is undefined behavior, and unsigned arithmetic wraps modulo 2ⁿ.
- `ckd_add`, `ckd_sub` and `ckd_mul` in `<stdckdint.h>` compute the result as if the type had infinite range and return `true` when the stored value is not the true result.
- Without `<stdckdint.h>`, check the operands before the operation, as in the fixed code of the page: `a > INT_MAX / b` for a product of positive values, and `a > INT_MAX - b` for a sum.
- Never check after the operation. A test such as `if (total < 0)` reads a value that the program had no right to compute.
