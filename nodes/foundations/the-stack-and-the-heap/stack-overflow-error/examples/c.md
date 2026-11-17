```c run id=deep
#include <stdio.h>

#define LIMIT 10000 /* well below the depth where the stack runs out */

/* Returns -1 when the depth passes LIMIT, so the stack never overflows. */
static long depth(long n, int level) {
    if (level > LIMIT) return -1;
    if (n == 0) return 0;
    long rest = depth(n - 1, level + 1);
    return rest < 0 ? rest : 1 + rest;
}

static long loop(long n) {
    long count = 0;
    while (n > 0) { /* one frame for the whole loop */
        n--;
        count++;
    }
    return count;
}

int main(void) {
    printf("depth(100) = %ld\n", depth(100, 1));
    if (depth(1000000, 1) < 0) {
        printf("depth(1000000): stopped at the limit of %d\n", LIMIT);
    }
    printf("loop(1000000) = %ld\n", loop(1000000));
    return 0;
}
```

```output for=deep
depth(100) = 100
depth(1000000): stopped at the limit of 10000
loop(1000000) = 1000000
```

- Verdict: native. C does not check the depth. A call past the end of the stack usually ends in a segmentation fault, and the program cannot recover. The program counts its own depth and stops first.
- `ulimit -s` in Bash sets the maximum stack size for the programs that the shell starts.
