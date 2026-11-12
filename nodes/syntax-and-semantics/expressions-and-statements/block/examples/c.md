```c run id=compound-statements-and-braces
#include <stdio.h>

static int calls;

static void hello(void) { calls++; }

int main(void) {
    int x = 1;
    {   /* a compound statement is a block */
        int x = 2;   /* this x hides the outer x */
        printf("inner x = %d\n", x);
    }
    printf("outer x = %d\n", x);

    /* A for statement is a block, so i is not visible after the loop. */
    int sum = 0;
    for (int i = 1; i <= 4; i++) sum += i;
    printf("sum = %d\n", sum);

    /* Without braces, only the first statement belongs to the if. */
    if (x > 5)
        hello();
        hello();   /* this call is outside the if, although the indent says otherwise */
    printf("calls = %d\n", calls);

    /* An automatic object lives until the end of its block. */
    int *p;
    {
        static int kept = 7;   /* static storage: it lives for the whole program */
        p = &kept;
    }
    printf("*p = %d\n", *p);
    return 0;
}
```

```output for=compound-statements-and-braces
inner x = 2
outer x = 1
sum = 10
calls = 1
*p = 7
```

- Verdict: native. A compound statement is a block, and an inner `x` hides the outer one. The `i` of a `for` loop is local to the loop. Without braces, an `if` controls only the next statement, so the second `hello()` runs although its indent says otherwise. A `static` object in a block lives for the whole program.
