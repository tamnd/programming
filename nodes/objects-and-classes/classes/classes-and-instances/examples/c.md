```c run id=struct-and-functions
#include <stdio.h>

/* The data: a struct. */
struct account {
    const char *owner;
    long balance;
};

/* A file-level variable plays the role of a static field. */
static int opened = 0;

/* The behavior: functions that take a pointer to the struct, like this. */
static void account_init(struct account *a, const char *owner, long balance)
{
    a->owner = owner;
    a->balance = balance;
    opened++;
}

static void account_deposit(struct account *a, long amount) { a->balance += amount; }

static double account_interest(const struct account *a) { return a->balance * 0.02; }

int main(void)
{
    struct account a, b;
    account_init(&a, "ana", 100);
    account_init(&b, "bo", 0);
    account_deposit(&a, 50);
    printf("%s %ld, %s %ld, %d\n", a.owner, a.balance, b.owner, b.balance, opened);
    printf("%.1f\n", account_interest(&a));
    return 0;
}
```

```output for=struct-and-functions
ana 150, bo 0, 2
3.0
```

- Verdict: idiom. C has no classes. A `struct` holds the data, and functions whose names start with the name of the type take a pointer to it as their first parameter, which plays the role of `this`. A `const` pointer marks a function that only reads. A `static` variable at file level is shared by all values of the type and hidden from other files. To hide the fields too, a header declares the struct without its members, and only the implementation file defines them.
