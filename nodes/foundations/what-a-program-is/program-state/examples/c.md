```c run id=caller-owned-state
#define _POSIX_C_SOURCE 200809L /* for strtok_r */
#include <stdio.h>
#include <string.h>

int main(void) {
    char text[] = "a=1,b=2;c=3";
    char *save_outer, *save_inner;
    /* strtok_r keeps its position in a variable that the caller owns */
    for (char *rec = strtok_r(text, ";", &save_outer); rec; rec = strtok_r(NULL, ";", &save_outer)) {
        printf("record:");
        for (char *f = strtok_r(rec, ",", &save_inner); f; f = strtok_r(NULL, ",", &save_inner))
            printf(" [%s]", f);
        printf("\n");
    }
    return 0;
}
```

```output for=caller-owned-state
record: [a=1] [b=2]
record: [c=3]
```

```c wrong id=hidden-static-state
#include <stdio.h>
#include <string.h>

int main(void) {
    char text[] = "a=1,b=2;c=3";
    /* strtok keeps its position in one hidden static variable */
    for (char *rec = strtok(text, ";"); rec; rec = strtok(NULL, ";")) {
        printf("record:");
        for (char *f = strtok(rec, ","); f; f = strtok(NULL, ","))
            printf(" [%s]", f);
        printf("\n");
    }
    return 0;
}
```

```output for=hidden-static-state
record: [a=1] [b=2]
```

- Verdict: native. C has three storage durations for state: automatic for locals, static for globals and `static` locals, and allocated for `malloc`. `strtok_r`, from POSIX, keeps its position in a pointer that the caller passes in, so two loops can tokenize at the same time. C11 has `strtok_s` in its optional Annex K.
- The wrong block uses `strtok`, which keeps its position in one hidden static variable. The inner loop moves that position to the end of the first record, so the outer loop stops and the record `c=3` is lost.
