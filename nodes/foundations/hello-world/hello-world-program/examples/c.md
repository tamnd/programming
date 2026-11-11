```c run id=int-main
#include <stdio.h>

int main(int argc, char *argv[]) {
    (void)argv;
    printf("Hello, world!\n");
    printf("%s has %d letters\n", "world", 5);
    printf("arguments: %d\n", argc - 1);
    return 0;
}
```

```output for=int-main
Hello, world!
world has 5 letters
arguments: 0
```

```c wrong id=underscore-exit-skips-flush
#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("start\n");
    fflush(stdout);
    printf("Hello, world!\n"); /* waits in the buffer when stdout is a pipe */
    _exit(0);                  /* ends the process without flushing stdio */
}
```

```output for=underscore-exit-skips-flush
start
```

- Verdict: native. A C program starts at `int main(void)` or `int main(int argc, char *argv[])`, and the value that `main` returns is the exit status. `printf` from `<stdio.h>` writes formatted text to standard output, and `\n` ends the line. Reaching the end of `main` returns 0.
- The wrong block writes a line and then calls `_exit`, which ends the process without flushing the buffers of `<stdio.h>`. When the output goes to a pipe or a file, the buffer still holds the line, and it is lost. Return from `main` or call `exit`, which flush the buffers.
