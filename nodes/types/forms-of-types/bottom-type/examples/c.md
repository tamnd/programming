```c run id=noreturn
#include <stdio.h>
#include <stdlib.h>

/* exit ends the process, so stop never returns to its caller. */
[[noreturn]] static void stop(const char *why) {
    printf("stop: %s\n", why);
    exit(0);
}

static int checked_div(int a, int b) {
    if (b == 0) {
        stop("division by zero");
    }
    return a / b;
}

int main(void) {
    printf("%d\n", checked_div(10, 2));
    printf("%d\n", checked_div(1, 0));
    printf("not reached\n");
    return 0;
}
```

```output for=noreturn
5
stop: division by zero
```

- Verdict: idiom. C has no bottom type. `[[noreturn]]`, or `_Noreturn` in C11, says that `stop` does not return, and the standard functions `exit` and `abort` are declared the same way. `stop` still has the type `void`. The call `checked_div(1, 0)` reaches `exit`, so the last `printf` never runs.
