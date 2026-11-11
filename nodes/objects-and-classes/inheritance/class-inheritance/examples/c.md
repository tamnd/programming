```c run id=embedding
#include <stdio.h>

struct account { const char *owner; int balance; };

/* The base comes first, so a pointer to a checking points to its account too. */
struct checking { struct account base; int overdraft; };

static void deposit(struct account *a, int n) { a->balance += n; }
static void describe(const struct account *a) { printf("account of %s: %d\n", a->owner, a->balance); }

static int can_withdraw(const struct checking *c, int n)
{
    return c->base.balance + c->overdraft >= n;
}

int main(void)
{
    struct checking c = { { "bo", 0 }, 100 };
    deposit(&c.base, 50);                    /* reuse the functions of the base */
    describe((const struct account *)&c);    /* valid: base is the first member */
    printf("withdraw 120: %d, withdraw 200: %d\n", can_withdraw(&c, 120), can_withdraw(&c, 200));
    return 0;
}
```

```output for=embedding
account of bo: 50
withdraw 120: 1, withdraw 200: 0
```

- Verdict: idiom. C has no classes, but a struct whose first member is another struct can stand in for a derived class. The C standard guarantees that a pointer to a struct, converted, points to its first member, so functions written for `struct account` work on a `struct checking`. With a table of function pointers in the base, this gives the whole object model of single inheritance. The Linux kernel, GLib and CPython's object headers use this layout.
- The compiler does not check the casts. Casting a pointer to a struct that does not start with the base reads the wrong memory.
