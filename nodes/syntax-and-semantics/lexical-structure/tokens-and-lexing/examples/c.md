```c run id=maximal-munch
#include <stdio.h>

int main(void) {
    int i = 1, j = 10;
    int k = i+++j; /* lexed as i ++ + j */
    printf("%d %d\n", k, i);

    printf("%d %d %d\n", 017, 0xff, 'A');
    const char *s = "hello, " "world"; /* adjacent strings join */
    printf("%s\n", s);
    int a = 8, *p = &a;
    int half = a / *p; /* a space keeps / * from starting a comment */
    printf("%d\n", half);
    return 0;
}
```

```output for=maximal-munch
11 2
15 255 65
hello, world
1
```

```c fails expect=compile id=preprocessing-number
#include <stdio.h>

int main(void) {
    int foo = 1;
    int bar = 0xE+foo; /* 0xE+foo is one preprocessing number */
    printf("%d\n", bar);
    return 0;
}
```

- Verdict: absent. C has no library for its own tokens; tools use libclang or a parser of their own. The preprocessor forms tokens by the longest match, so `i+++j` is `i++ + j`, and `a/*p` would start a comment, which is why the example puts a space after the `/`. Adjacent string literals join into one, which lets a long string span several lines.
- The fails block writes `0xE+foo`. A preprocessing number can contain `e+` and `E+` for exponents, so the longest match makes `0xE+foo` one token, which is not a valid number, and the compiler rejects it. Spaces around `+` fix it.
