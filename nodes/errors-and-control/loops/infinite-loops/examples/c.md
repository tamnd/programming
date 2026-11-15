```c run id=forever-wrap-and-cycle-check
#include <stdio.h>

int main(void) {
    /* for (;;) is read "forever"; break is the only exit. */
    int n = 1, steps = 0;
    for (;;) {
        n *= 2;
        steps++;
        if (n > 1000)
            break;
    }
    printf("n=%d after %d steps\n", n, steps);

    /* A pseudo-infinite loop: the counter wraps to 0 and the loop ends. */
    int passes = 0;
    for (unsigned char c = 250; c != 0; c++)
        passes++;
    printf("unsigned char loop: %d passes\n", passes);

    /* Floyd's cycle check on next indexes: 0 -> 1 -> 2 -> 3 -> 1. */
    int next[] = {1, 2, 3, 1};
    int slow = 0, fast = 0;
    do {
        slow = next[slow];
        fast = next[next[fast]];
    } while (slow != fast);
    printf("walkers meet at node %d\n", slow);
    return 0;
}
```

```output for=forever-wrap-and-cycle-check
n=1024 after 10 steps
unsigned char loop: 6 passes
walkers meet at node 3
```

- Verdict: idiom. C has no statement for an infinite loop, and `for (;;)` is the traditional form. The `unsigned char` loop looks endless, but the counter wraps from 255 to 0 after 6 passes. A plain walk along `next` from node 0 would never end, and the two walkers of the cycle check meet inside the cycle.
