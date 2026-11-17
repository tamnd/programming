```c run id=trace
#include <stdio.h>

/* C has no standard way to read the call stack. The program keeps its own copy:
   each function pushes its name on entry and pops it before it returns. */
static const char *names[16];
static int depth = 0;

#define ENTER(name) (names[depth++] = (name))
#define LEAVE() (depth--)

static void where(void) {
    printf("      stack:");
    for (int i = depth - 1; i >= 0; i--) {
        printf(i == depth - 1 ? " %s" : " <- %s", names[i]);
    }
    printf("\n");
}

static void c(void) {
    ENTER("c");
    where();
    LEAVE();
}

static void b(void) {
    ENTER("b");
    printf("    call c\n");
    c();
    printf("    back in b\n");
    LEAVE();
}

static void a(void) {
    ENTER("a");
    printf("  call b\n");
    b();
    printf("  back in a\n");
    LEAVE();
}

int main(void) {
    ENTER("main");
    printf("call a\n");
    a();
    printf("back in main\n");
    LEAVE();
    return 0;
}
```

```output for=trace
call a
  call b
    call c
      stack: c <- b <- a <- main
    back in b
  back in a
back in main
```

- Verdict: native. The compiled program uses the call stack, but the C standard has no function to read it. A debugger such as GDB shows it with the `backtrace` command.
- The `names` array copies the push on call and the pop on return by hand.
