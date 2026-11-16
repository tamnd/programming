```c run id=fall-through-c
#include <stdio.h>

static void rights(int level) {
    printf("%d:", level);
    switch (level) {
    case 3:
        printf(" delete");
        [[fallthrough]];
    case 2:
        printf(" write");
        [[fallthrough]];
    case 1:
        printf(" read");
        break;
    default:
        printf(" none");
        break;
    }
    printf("\n");
}

int main(void) {
    rights(3);
    rights(1);
    rights(0);
    return 0;
}
```

```output for=fall-through-c
3: delete write read
1: read
0: none
```

- Verdict: native. A C case without `break` falls through into the next case. The C23 statement `[[fallthrough]];` marks the fall-through as intentional, so a compiler that warns on fall-through does not warn here.
