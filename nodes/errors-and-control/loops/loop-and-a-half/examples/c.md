```c run id=strtol-loop-and-getchar-style-condition
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    /* for (;;) with the exit in the middle: parse, test, add. */
    const char *s = "12 7 30 x 5";
    char *end;
    long sum = 0;
    for (;;) {
        long v = strtol(s, &end, 10);
        if (end == s)
            break; /* no number was read */
        sum += v;
        s = end;
    }
    printf("sum: %ld, stopped at \"%s\"\n", sum, s);

    /* The read in the condition, in the style of while ((c = getchar()) != EOF). */
    const char *p = "a bc def";
    int c, letters = 0;
    while ((c = *p++) != '\0') {
        if (c != ' ')
            letters++;
    }
    printf("letters: %d\n", letters);
    return 0;
}
```

```output for=strtol-loop-and-getchar-style-condition
sum: 49, stopped at " x 5"
letters: 6
```

- Verdict: idiom. `strtol` reads a number and sets `end` to the first character it did not use. If `end` equals the start, no number was read, and the loop leaves in the middle, before it adds anything. The second loop reads a character and tests it in one condition, the form of the classic `getchar` loop. The parentheses around the assignment matter, because `!=` binds tighter than `=`.
