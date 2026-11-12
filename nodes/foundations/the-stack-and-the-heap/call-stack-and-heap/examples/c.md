```c run id=automatic-static-and-allocated
#include <stdio.h>
#include <stdlib.h>

static int calls; /* static storage: one for the whole run */

int *squares(int n) {
    int *out = malloc(n * sizeof *out); /* heap: lives until free */
    for (int i = 0; i < n; i++) out[i] = i * i;
    return out;
}

long depth(int n) {
    calls++;
    int local = n; /* automatic storage: one per frame */
    return local == 0 ? 0 : 1 + depth(local - 1);
}

int main(void) {
    int *s = squares(4);
    printf("%d\n", s[3]);
    free(s); /* the program, not the language, ends the lifetime */

    long d = depth(1000); /* call first: C does not fix the order of arguments */
    printf("%ld %d\n", d, calls);

    int a[256]; /* 1 KiB in the frame of main */
    printf("%zu %zu\n", sizeof a, sizeof s);
    return 0;
}
```

```output for=automatic-static-and-allocated
9
1000 1001
1024 8
```

- Verdict: native. The C standard does not mention a stack or a heap. It speaks of storage durations: automatic for local variables, which end when their block ends, static for variables that exist for the whole run, and allocated for memory from `malloc`, which exists until `free`. Implementations put automatic storage in stack frames.
- Returning the address of a local variable compiles, with a warning, but the pointer dangles after the return. Return a value, or memory from `malloc` that the caller frees. The default stack of a thread is a few MiB on common systems, so large arrays belong on the heap.
