```c run id=command-letters
#include <stdio.h>

static const char *action(char command) {
    switch (command) {
    case 'q':
    case 'x':
        return "quit";
    case 'h':
    case '?':
        return "help";
    case 's':
        return "save";
    default:
        return "unknown";
    }
}

int main(void) {
    const char commands[] = "qhs?z";
    for (int i = 0; commands[i] != '\0'; i++) {
        printf("%c: %s\n", commands[i], action(commands[i]));
    }

    // With no default, a value that matches no label runs nothing.
    int n = 7;
    switch (n) {
    case 1:
        printf("one\n");
        break;
    }
    printf("after the switch\n");
    return 0;
}
```

```output for=command-letters
q: quit
h: help
s: save
?: help
z: unknown
after the switch
```

- Verdict: native. A C `switch` tests an integer, and a `char` is an integer, so the labels are character constants. Two labels in front of one statement share it. Each case ends with `return`, so no case runs into the next one. The second `switch` has no `default`, so the value 7 runs no part of its body.
