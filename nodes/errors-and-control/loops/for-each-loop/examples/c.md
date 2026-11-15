```c run id=length-sentinel-and-pointer-loops
#include <stddef.h>
#include <stdio.h>

#define COUNT(a) (sizeof (a) / sizeof (a)[0])

int main(void) {
    int primes[] = {2, 3, 5, 7};
    int sum = 0;
    for (size_t i = 0; i < COUNT(primes); i++)
        sum += primes[i];
    printf("sum: %d\n", sum);

    /* A NULL at the end marks the last element. */
    const char *names[] = {"ann", "bob", NULL};
    for (const char **p = names; *p != NULL; p++)
        printf("name: %s\n", *p);

    /* A pointer from the start to one past the end. */
    for (int *p = primes, *end = primes + COUNT(primes); p != end; p++)
        *p *= 2;
    printf("%d %d %d %d\n", primes[0], primes[1], primes[2], primes[3]);
    return 0;
}
```

```output for=length-sentinel-and-pointer-loops
sum: 17
name: ann
name: bob
4 6 10 14
```

- Verdict: absent. C has no for-each loop, and an array passed to a function loses its length. The three loops are the usual substitutes: an index up to a length computed with `sizeof`, a pointer up to a `NULL` sentinel, and a pointer up to one past the end, the model for the iterators of C++.
