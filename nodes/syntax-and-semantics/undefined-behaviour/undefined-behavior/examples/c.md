```c fails expect=sanitizer:undefined timeout=30 id=signed-overflow
#include <limits.h>
#include <stdio.h>

int next(int x) {
    return x + 1; // undefined when x is INT_MAX
}

int main(void) {
    printf("%d\n", next(41));
    printf("%d\n", next(INT_MAX));
}
```

UndefinedBehaviorSanitizer stops the program at the second call with the report "signed integer overflow". Without the sanitizer, the program has no defined output. Do not trust a value that an unsanitized build prints.

```c fails expect=sanitizer:undefined timeout=30 id=null-check
#include <stddef.h>
#include <stdio.h>

struct node {
    int value;
};

int value_or_zero(struct node *n) {
    int v = n->value; // the dereference comes first
    if (n == NULL) {  // so the compiler may remove this check
        return 0;
    }
    return v;
}

int main(void) {
    struct node one = {7};
    printf("%d\n", value_or_zero(&one));
    printf("%d\n", value_or_zero(NULL));
}
```

The sanitizer reports a member access within a null pointer. The check `n == NULL` comes after `n->value`, so an optimizer may delete it. GCC does this by default with `-fdelete-null-pointer-checks`.

```c fails expect=sanitizer:undefined timeout=30 id=shift
#include <stdio.h>

unsigned int bit(int n) {
    return 1u << n; // undefined when n is 32 or more
}

int main(void) {
    printf("%u\n", bit(3));
    printf("%u\n", bit(32));
}
```

A shift by the width of the type or more is undefined, also for unsigned types. Hardware differs here, so the standard does not pick one result.

```c run flags="-std=c23 -fsanitize=undefined" id=defined
#include <limits.h>
#include <stdckdint.h>
#include <stddef.h>
#include <stdio.h>

struct node {
    int value;
};

int value_or_zero(const struct node *n) {
    if (n == NULL) { // check before the dereference
        return 0;
    }
    return n->value;
}

int main(void) {
    int sum;
    if (ckd_add(&sum, INT_MAX, 1)) {
        printf("INT_MAX + 1 does not fit in int\n");
    }
    printf("value_or_zero(NULL) = %d\n", value_or_zero(NULL));
    unsigned int n = 32;
    printf("1u << %u = %u\n", n, n < 32 ? 1u << n : 0u);
}
```

```output for=defined
INT_MAX + 1 does not fit in int
value_or_zero(NULL) = 0
1u << 32 = 0
```

```c run id=implementation-defined
#include <stdio.h>

int main(void) {
    printf("sizeof(int) = %zu\n", sizeof(int));
    printf("-7 >> 1 = %d\n", -7 >> 1);
}
```

```output for=implementation-defined
sizeof(int) = 4
-7 >> 1 = -4
```

- Verdict: native. The C standard defines undefined behavior in 3.5.3 and lists the cases in Annex J.2.
- `ckd_add`, `ckd_sub` and `ckd_mul` from `<stdckdint.h>` return `true` when the result does not fit. The program then never computes the overflowing value.
- The size of `int` and the result of `-7 >> 1` are implementation-defined. Each implementation documents its choice, and the output above is the choice of the compiler that recorded it.
- The defined program also runs under `-fsanitize=undefined`, and the sanitizer reports nothing.
