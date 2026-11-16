```c run id=infinite
#include <stdbool.h>
#include <stdio.h>

struct naturals {
    long next;
};

static long naturals_next(struct naturals *g) {
    return g->next++;
}

static bool is_prime(long n) {
    for (long d = 2; d * d <= n; d++) {
        if (n % d == 0) {
            return false;
        }
    }
    return true;
}

struct primes {
    struct naturals source;
};

static long primes_next(struct primes *g) {
    for (;;) {
        long n = naturals_next(&g->source);
        if (is_prime(n)) {
            return n;
        }
    }
}

int main(void) {
    struct primes g = {{2}};
    for (int i = 0; i < 10; i++) {
        printf("%ld ", primes_next(&g));
    }
    printf("\n");

    long x = 1;
    for (int i = 0; i < 8; i++) {
        printf("%ld ", x);
        x *= 2;
    }
    printf("\n");
    return 0;
}
```

```output for=infinite
2 3 5 7 11 13 17 19 23 29 
1 2 4 8 16 32 64 128 
```

- Verdict: absent. C has no generators. A struct holds the state of the infinite source, and a `next` function computes one element at each call. The caller's loop bound is the only limit.
- `primes` wraps `naturals`: it pulls numbers until one is prime. A `long` overflows after enough calls, so the source is only infinite in principle.
