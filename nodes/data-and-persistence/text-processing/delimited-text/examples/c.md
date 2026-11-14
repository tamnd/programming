```c run id=strtok-and-strsep
#define _DEFAULT_SOURCE /* strsep */
#include <stdio.h>
#include <string.h>

int main(void) {
    char a[] = "a::b:";
    char b[] = "a::b:";

    /* strtok skips runs of delimiters, so it loses the empty fields. */
    int n = 0;
    for (char *t = strtok(a, ":"); t != NULL; t = strtok(NULL, ":")) {
        printf("strtok %d: \"%s\"\n", ++n, t);
    }

    /* strsep returns each field, also the empty ones. */
    n = 0;
    char *rest = b;
    char *t;
    while ((t = strsep(&rest, ":")) != NULL) {
        printf("strsep %d: \"%s\"\n", ++n, t);
    }
    return 0;
}
```

```output for=strtok-and-strsep
strtok 1: "a"
strtok 2: "b"
strsep 1: "a"
strsep 2: ""
strsep 3: "b"
strsep 4: ""
```

- Verdict: stdlib. `strtok` treats a run of delimiters as one, so it loses the empty fields, and it writes into the string. `strsep` is not in ISO C, but glibc and the BSDs have it, and it returns each field, also the empty ones.
