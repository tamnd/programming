```c run id=light
#include <stdio.h>

enum light { RED, AMBER, GREEN };

static const char *const names[] = {"red", "amber", "green"};

static enum light after(enum light l) {
    switch (l) {
    case RED:
        return GREEN;
    case GREEN:
        return AMBER;
    case AMBER:
        return RED;
    }
    return RED;
}

int main(void) {
    for (int i = RED; i <= GREEN; i++) {
        enum light l = (enum light)i;
        printf("%s %d then %s\n", names[l], i, names[after(l)]);
    }
    printf("sum: %d\n", RED + AMBER + GREEN);
    return 0;
}
```

```output for=light
red 0 then green
amber 1 then red
green 2 then amber
sum: 3
```

- Verdict: native. An `enum` declares named `int` constants, here 0, 1 and 2.
- C has no names at run time, so the program keeps an array of names in the same order.
- The constants are plain integers, so `RED + AMBER + GREEN` compiles.
