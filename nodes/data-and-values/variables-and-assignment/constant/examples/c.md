```c run id=const
#include <stdio.h>

#define MAX_SIZE 10

enum { DAYS_PER_WEEK = 7 };

int main(void) {
    const double rate = 0.2;
    const char *name = "Ada";
    name = "Bob";
    int counts[MAX_SIZE] = {0};
    printf("%s %d %.1f %zu\n", name, DAYS_PER_WEEK, rate, sizeof counts / sizeof counts[0]);
    return 0;
}
```

```output for=const
Bob 7 0.2 10
```

- Verdict: native. The qualifier `const` stops writes to an object: `rate = 0.3` does not compile. In `const char *name`, the characters are constant but the pointer is not, so `name = "Bob"` is fine; `char *const` makes the pointer constant. A `#define` macro or an `enum` gives a constant that can size an array.
