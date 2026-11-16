```c run id=exhaustive-matching-c
#include <stdio.h>

enum light { RED, YELLOW, GREEN };

static const char *action(enum light light) {
    /* No default: -Wswitch (in -Wall) warns if an enumerator has no case. */
    switch (light) {
    case RED:
        return "stop";
    case YELLOW:
        return "slow down";
    case GREEN:
        return "go";
    }
    return "unknown";
}

int main(void) {
    const char *names[] = {"red", "yellow", "green"};
    for (int i = RED; i <= GREEN; i++) {
        printf("%s: %s\n", names[i], action((enum light)i));
    }
    return 0;
}
```

```output for=exhaustive-matching-c
red: stop
yellow: slow down
green: go
```

- Verdict: idiom. The C language does not require a switch to be exhaustive. GCC warns with `-Wswitch`, which `-Wall` turns on, when a switch on an enum has no case for a named enumerator. A `default` label turns the warning off, so the switch has no `default`. The `return` after the switch handles a value that is not a named enumerator.
