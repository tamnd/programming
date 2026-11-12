```c run id=assert-with-source-text
#include <stdio.h>

/* #e turns the macro argument into a string literal, and __LINE__ is the line of the use. */
#define CHECK(e) printf("line %d in %s: %s is %s\n", __LINE__, __func__, #e, (e) ? "true" : "false")

/* Text that the preprocessor joins: a backslash at the end of a line continues the line. */
#define AREA(w, h) \
    ((w) * (h))

int main(void) {
    int x = -2;
    CHECK(x > 0);
    CHECK(x * x == 4);
    printf("AREA(2 + 1, 4) = %d\n", AREA(2 + 1, 4));
    printf("adjacent " "string " "literals join: %s\n", "yes");
    return 0;
}
```

```output for=assert-with-source-text
line 12 in main: x > 0 is false
line 13 in main: x * x == 4 is true
AREA(2 + 1, 4) = 12
adjacent string literals join: yes
```

- Verdict: native. The preprocessor works on the source text before the compiler sees it. `#e` turns the macro argument into a string literal, `__LINE__` gives the line of the use and `__func__` gives the name of the function, as the standard `assert` does. A backslash at the end of a line joins it to the next line, and adjacent string literals join into one.
