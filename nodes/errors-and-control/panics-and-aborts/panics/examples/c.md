```c run id=exit
#include <stdio.h>
#include <stdlib.h>

static void close_log(void) { puts("atexit: close log"); }
static void flush_cache(void) { puts("atexit: flush cache"); }

static void stop(int status) {
    puts("stopping");
    exit(status);
}

int main(void) {
    atexit(close_log);
    atexit(flush_cache);
    stop(EXIT_SUCCESS);
    puts("not reached");
}
```

```output
stopping
atexit: flush cache
atexit: close log
```

- Verdict: idiom. C has no panic and no unwinding. A program stops with `exit` or with `abort`.
- `exit` calls the functions registered with `atexit` in the reverse order of registration. It then flushes and closes the open streams.
- `abort` raises `SIGABRT`. It does not run the `atexit` functions, and the standard leaves it to the implementation whether it flushes the streams. A failed `assert` calls `abort`.

`abort` ends the program without the `atexit` functions.

```c fails expect=runtime id=abort
#include <stdio.h>
#include <stdlib.h>

static void close_log(void) { puts("atexit: close log"); }

int main(void) {
    atexit(close_log);
    fputs("broken invariant\n", stderr);
    abort();
}
```

A failed `assert` writes the expression, the file and the line to standard error, then calls `abort`.

```c fails expect=runtime id=assert
#include <assert.h>
#include <stddef.h>

static int item(const int *items, size_t len, size_t i) {
    assert(i < len);
    return items[i];
}

int main(void) {
    int items[] = {1, 2};
    return item(items, 2, 5);
}
```
