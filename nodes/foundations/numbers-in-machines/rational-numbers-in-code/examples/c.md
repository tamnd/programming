```c run id=ratio
#include <stdio.h>

typedef struct {
    long long n;
    long long d;
} ratio;

static long long gcd(long long a, long long b) {
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a < 0 ? -a : a;
}

static int make(long long n, long long d, ratio *out) {
    if (d == 0) {
        return -1;
    }
    if (d < 0) {
        n = -n;
        d = -d;
    }
    long long g = gcd(n, d);
    out->n = n / g;
    out->d = d / g;
    return 0;
}

static int add(ratio a, ratio b, ratio *out) {
    long long x = 0;
    long long y = 0;
    long long n = 0;
    long long d = 0;
    if (__builtin_mul_overflow(a.n, b.d, &x) || __builtin_mul_overflow(b.n, a.d, &y) ||
        __builtin_add_overflow(x, y, &n) || __builtin_mul_overflow(a.d, b.d, &d)) {
        return -1;
    }
    return make(n, d, out);
}

int main(void) {
    ratio r = {0, 1};
    make(6, -8, &r);
    printf("make(6, -8): %lld/%lld\n", r.n, r.d);
    ratio a = {0, 1};
    ratio b = {0, 1};
    make(1, 10, &a);
    make(2, 10, &b);
    add(a, b, &r);
    printf("1/10 + 2/10: %lld/%lld\n", r.n, r.d);
    printf("0.1 + 0.2 == 0.3: %s\n", 0.1 + 0.2 == 0.3 ? "yes" : "no");
    printf("make(1, 0): %s\n", make(1, 0, &r) == 0 ? "ok" : "error");
    ratio h = {0, 1};
    for (long long k = 1; k <= 20; k++) {
        ratio t = {0, 1};
        make(1, k, &t);
        add(h, t, &h);
    }
    printf("1/1 + ... + 1/20: %lld/%lld\n", h.n, h.d);
    return 0;
}
```

```output for=ratio
make(6, -8): -3/4
1/10 + 2/10: 3/10
0.1 + 0.2 == 0.3: no
make(1, 0): error
1/1 + ... + 1/20: 55835135/15519504
```

- Verdict: library. C has no rational type, and GMP has the `mpq_t` type, with bignum parts.
- The block uses `long long` parts and returns -1 on a zero denominator or an overflow, so the caller must check each result.
