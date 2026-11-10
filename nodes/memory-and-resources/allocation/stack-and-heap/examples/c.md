```c run id=storage
#include <stdio.h>
#include <stdlib.h>

int factorial(int n) {
    int result = 1; // each call has its own result
    if (n > 1) {
        result = n * factorial(n - 1);
    }
    return result;
}

int *make_totals(size_t count) {
    int *totals = malloc(count * sizeof *totals);
    if (totals != NULL) {
        for (size_t i = 0; i < count; i++) {
            totals[i] = (int)(i * 10);
        }
    }
    return totals;
}

int main(void) {
    printf("%d\n", factorial(5));
    int *totals = make_totals(3);
    if (totals == NULL) {
        return 1;
    }
    printf("%d %d %d\n", totals[0], totals[1], totals[2]);
    free(totals);
}
```

```output
120
0 10 20
```

- Verdict: native. The C standard speaks of automatic and allocated storage durations, not of a stack and a heap.
- Each call of `factorial` has its own `result`, because a recursive call makes a separate instance of each automatic object.
- The block from `malloc` outlives `make_totals`. It lives until `free`, and the caller must call `free` exactly once. See [[explicit-memory-management]].
- Returning the address of a local is undefined behaviour. The Pitfalls section shows how AddressSanitizer finds it.
