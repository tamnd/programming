```c run id=bitwise-and-comparison
#include <stdio.h>

int main(void) {
    int x = 6;
    /* == binds more tightly than &, so x & 1 == 0 means x & (1 == 0), which is always 0. */
    printf("%d %d\n", x & 1 == 0, (x & 1) == 0);

    /* + binds more tightly than <<. */
    printf("%d\n", 1 << 2 + 1);

    /* Postfix ++ binds more tightly than unary *, so *p++ reads *p and then moves p. */
    int xs[] = {10, 20, 30};
    int *p = xs;
    int first = *p++;
    printf("%d %d\n", first, *p);
    (*p)++; /* parentheses increment the value instead */
    printf("%d\n", xs[1]);

    /* a < b < c is (a < b) < c, and a < b is 0 or 1. */
    int a = 3, b = 2, c = 1;
    printf("%d\n", a < b < c);

    /* sizeof binds like a unary operator: sizeof x + 1 is (sizeof x) + 1. */
    printf("%zu\n", sizeof x + 1);

    /* The conditional operator groups to the right. */
    int n = 10;
    printf("%s\n", n % 15 == 0 ? "FizzBuzz" : n % 3 == 0 ? "Fizz" : n % 5 == 0 ? "Buzz" : "number");
    return 0;
}
```

```output for=bitwise-and-comparison
0 1
8
10 20
21
1
5
Buzz
```

- Verdict: native. `==` binds more tightly than `&`, so `x & 1 == 0` is `x & (1 == 0)`, which is 0 for every `x`. `+` binds more tightly than `<<`. Postfix `++` binds more tightly than unary `*`, so `*p++` moves the pointer. `a < b < c` compares 0 or 1 with `c`. Clang with `-Wall` warns about all three of these mixes, and the program compiles with the warnings and prints the results.
