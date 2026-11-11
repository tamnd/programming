```c run id=opaque-pointer
#include <stdio.h>
#include <stdlib.h>

/* The "header": callers see an incomplete type and functions, not the fields. */
struct account;
struct account *account_new(void);
int account_deposit(struct account *a, long amount);
int account_withdraw(struct account *a, long amount);
long account_balance(const struct account *a);
void account_free(struct account *a);

int main(void)
{
    struct account *a = account_new();
    account_deposit(a, 100);
    account_withdraw(a, 30);
    int refused = account_withdraw(a, 500);
    printf("%ld %d\n", account_balance(a), refused);
    account_free(a);
    return 0;
}

/* The "implementation": in a real program, a separate .c file. */
struct account {
    long balance;
};

struct account *account_new(void) { return calloc(1, sizeof(struct account)); }

int account_deposit(struct account *a, long amount)
{
    if (amount <= 0)
        return -1;
    a->balance += amount;
    return 0;
}

int account_withdraw(struct account *a, long amount)
{
    if (amount > a->balance)
        return -1;
    a->balance -= amount;
    return 0;
}

long account_balance(const struct account *a) { return a->balance; }
void account_free(struct account *a) { free(a); }
```

```output for=opaque-pointer
70 -1
```

```c fails expect=compile id=incomplete-type
#include <stdio.h>

struct account; /* incomplete: the fields are not visible here */
struct account *account_new(void);

int main(void)
{
    struct account *a = account_new();
    a->balance = 1000000; /* the compiler does not know the layout */
    return 0;
}
```

- Verdict: idiom. C has no access control on struct members. The opaque pointer idiom gives the same effect: the header declares `struct account` without its members, and functions that take a pointer to it. Code that includes only the header cannot read or write the fields, because it does not know the layout, and it cannot create an `account` on the stack, so a constructor function allocates it. The `FILE` type of the standard library works this way. Functions marked `static` are private to their source file.
- The failing block, which sees only the declaration, tries to write a field through the pointer, and the compiler rejects it because the type is incomplete.
