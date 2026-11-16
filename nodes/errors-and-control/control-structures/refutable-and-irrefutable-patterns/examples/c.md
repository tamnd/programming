```c run id=refutable-c
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *lines[] = {"width=80", "oops", "height=24"};
    for (int i = 0; i < 3; i++) {
        const char *eq = strchr(lines[i], '=');
        if (eq == NULL) {
            printf("skip: %s\n", lines[i]);
            continue;
        }
        printf("%.*s -> %s\n", (int)(eq - lines[i]), lines[i], eq + 1);
    }
    return 0;
}
```

```output for=refutable-c
width -> 80
skip: oops
height -> 24
```

- Verdict: absent. C has no patterns. The program tests the result of `strchr` for `NULL`, and then reads the two parts by position.
