```c run id=bignum
#include <stdint.h>
#include <stdio.h>

#define CAP 64
#define BASE 1000000000u

typedef struct {
    uint32_t d[CAP];
    int n;
} big;

static void big_one(big *b) {
    b->d[0] = 1;
    b->n = 1;
}

static int big_mul_small(big *b, uint32_t m) {
    uint64_t carry = 0;
    for (int i = 0; i < b->n; i++) {
        uint64_t t = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)(t % BASE);
        carry = t / BASE;
    }
    while (carry > 0) {
        if (b->n == CAP) {
            return -1;
        }
        b->d[b->n++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
    return 0;
}

static void big_print(const char *label, const big *b) {
    printf("%s: %u", label, (unsigned)b->d[b->n - 1]);
    for (int i = b->n - 2; i >= 0; i--) {
        printf("%09u", (unsigned)b->d[i]);
    }
    printf("\n");
}

int main(void) {
    uint64_t f = 1;
    uint64_t next = 0;
    unsigned n = 1;
    while (!__builtin_mul_overflow(f, (uint64_t)n + 1, &next)) {
        f = next;
        n++;
    }
    printf("largest n! in uint64_t: %u! = %llu\n", n, (unsigned long long)f);

    big b;
    big_one(&b);
    for (uint32_t i = 1; i <= 30; i++) {
        big_mul_small(&b, i);
    }
    big_print("30!", &b);
    big_one(&b);
    for (int i = 0; i < 100; i++) {
        big_mul_small(&b, 2);
    }
    big_print("2^100", &b);
    return 0;
}
```

```output for=bignum
largest n! in uint64_t: 20! = 2432902008176640000
30!: 265252859812191058636308480000000
2^100: 1267650600228229401496703205376
```

- Verdict: library. C has no bignum type, and GMP is the usual library, with the `mpz_t` type for integers.
- The block keeps a number as an array of base 10^9 digits. The fixed `CAP` keeps the code short, and `big_mul_small` returns -1 when the array is full.
- `__builtin_mul_overflow` reports the overflow of a fixed-width product, so the first loop finds the largest factorial that fits.
