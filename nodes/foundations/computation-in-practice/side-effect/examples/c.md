```c run id=static-state-and-macro
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b)) /* a macro copies its arguments */

static int next_id(void) {
    static int id = 0; /* keeps its value between calls */
    return ++id;
}

static void scale(int *xs, int n, int k) { /* writes through the pointer */
    for (int i = 0; i < n; i++)
        xs[i] *= k;
}

int main(void) {
    int a = next_id(); /* one call per statement: the order is clear */
    int b = next_id();
    printf("ids %d %d\n", a, b);

    int xs[] = {1, 2, 3};
    scale(xs, 3, 10);
    printf("scaled %d %d %d\n", xs[0], xs[1], xs[2]);

    int before = next_id();
    int m = MAX(0, next_id()); /* next_id() runs twice */
    printf("last id %d, then MAX(0, next_id()) gave %d\n", before, m);
    return 0;
}
```

```output for=static-state-and-macro
ids 1 2
scaled 10 20 30
last id 3, then MAX(0, next_id()) gave 5
```

- Verdict: native. A `static` local variable keeps its value between calls, so `next_id` returns a new number each time. A function that takes a pointer can write through it; a `const` pointer parameter promises that it does not. The last line shows a macro trap: `MAX(0, next_id())` puts `next_id()` in the text twice, so it runs twice and skips the id 4. The `?:` operator has a sequence point after its first operand, so this case is defined; a macro that uses an argument twice in one unsequenced expression, such as `SQUARE(i++)`, is undefined behavior.
