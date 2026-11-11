```c run id=account
#include <assert.h>
#include <stdio.h>

struct account {
    int balance;
};

static_assert(sizeof(int) >= 2, "int has at least 16 bits");

static void withdraw(struct account *a, int amount) {
    assert(a != NULL);
    assert(amount > 0 && amount <= a->balance);
    int old = a->balance;
    a->balance -= amount;
    assert(a->balance == old - amount);
    assert(a->balance >= 0);
}

int main(void) {
    struct account a = {100};
    withdraw(&a, 30);
    printf("balance %d\n", a.balance);
#ifdef NDEBUG
    puts("NDEBUG is defined");
#else
    puts("NDEBUG is not defined");
#endif
}
```

```output
balance 70
NDEBUG is not defined
```

- Verdict: stdlib. The macro `assert` in `<assert.h>` writes the failed expression, the file, the line and the function to standard error, then calls `abort`.
- If `NDEBUG` is defined where `<assert.h>` is included, `assert` expands to `((void)0)`. The expression is not evaluated.
- `static_assert` is checked by the compiler. It costs nothing at run time.

A precondition that fails stops the program.

```c fails expect=runtime id=violated
#include <assert.h>
#include <stdio.h>

struct account {
    int balance;
};

static void withdraw(struct account *a, int amount) {
    assert(amount > 0 && amount <= a->balance);
    a->balance -= amount;
}

int main(void) {
    struct account a = {100};
    withdraw(&a, 500);
    printf("balance %d\n", a.balance);
}
```

With `-DNDEBUG`, the same program runs on with a negative balance.

```c wrong id=ndebug flags="-DNDEBUG"
#include <assert.h>
#include <stdio.h>

struct account {
    int balance;
};

static void withdraw(struct account *a, int amount) {
    assert(amount > 0 && amount <= a->balance);
    a->balance -= amount;
}

int main(void) {
    struct account a = {100};
    withdraw(&a, 500);
    printf("balance %d\n", a.balance);
}
```

```output
balance -400
```

A compile-time assertion that is false stops the build.

```c fails expect=compile id=static
#include <assert.h>

struct header {
    char tag;
    int length;
};

static_assert(sizeof(struct header) == 5, "header is 5 bytes");

int main(void) { return 0; }
```
