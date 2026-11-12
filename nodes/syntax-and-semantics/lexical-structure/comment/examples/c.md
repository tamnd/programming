```c run id=line-splice-and-c99
#include <stdio.h>

int main(void) {
    int n = 1;
    // A backslash at the end of a line comment joins the next line to it: \
    n = n + 1;
    printf("n = %d\n", n);

    int/**/x = 5; /* the compiler replaces a comment with one space */
    printf("x = %d\n", x);

    /* Block comments do not nest in C. The first end marker closes this comment: /* inner */
    printf("%s\n", "/* in a string, this is text */");

    int a = 8, b = 2;
    printf("%d\n", a //* a line comment starts here in C99, so this is a / b in C89 */ b
        + 0);
    (void)b;
    return 0;
}
```

```output for=line-splice-and-c99
n = 1
x = 5
/* in a string, this is text */
8
```

- Verdict: native. C has `/* */` and, since C99, `//`. Comments are removed in translation phase 3, and each one becomes one space, so `int/**/x` declares `x`. A backslash at the end of a `//` line joins the next line to the comment, so `n = n + 1;` does not run, and clang warns with `-Wcomment`. Block comments do not nest. In C89, `a //* ... */ b` meant `a / b`, but since C99 `//` starts a comment, so the program prints `8`.
