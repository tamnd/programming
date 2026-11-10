```c run id=text
#include <stdio.h>

#define SHOW(expr) printf("%s = %d\n", #expr, (expr))

struct point {
    int x;
    int y;
};

#define GETTER(field) \
    int get_##field(const struct point *p) { return p->field; }

GETTER(x)
GETTER(y)

int main(void) {
    struct point p = {3, 4};
    SHOW(get_x(&p) + get_y(&p));
    SHOW(p.x * p.y);
}
```

```output for=text
get_x(&p) + get_y(&p) = 7
p.x * p.y = 12
```

The `#` operator turns the argument into a string literal, and `##` joins two tokens into one identifier. A function cannot do either of these things, because it receives values, not program text.

```c wrong id=parentheses
#include <stdio.h>

#define SQUARE(x) x * x

int main(void) {
    printf("SQUARE(1 + 2) = %d\n", SQUARE(1 + 2));
}
```

```output for=parentheses
SQUARE(1 + 2) = 5
```

The expansion is `1 + 2 * 1 + 2`, which is 5, not 9. The fix is a pair of parentheses around each use of a parameter and around the whole body: `#define SQUARE(x) ((x) * (x))`.

```c wrong id=double-evaluation
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(void) {
    int i = 5;
    int m = MAX(i++, 0);
    printf("m = %d, i = %d\n", m, i);
}
```

```output for=double-evaluation
m = 6, i = 7
```

The argument `i++` appears twice in the expansion, so it runs twice when it is the larger value. A caller expects `m = 5` and `i = 6`. Parentheses cannot fix this.

```c wrong id=capture
#include <stdio.h>

#define SWAP(a, b) do { int tmp = (a); (a) = (b); (b) = tmp; } while (0)

int main(void) {
    int x = 1;
    int tmp = 2;
    SWAP(x, tmp);
    printf("x = %d, tmp = %d\n", x, tmp);
}
```

```output for=capture
x = 1, tmp = 2
```

The macro declares its own `tmp`, and that declaration hides the caller's `tmp` inside the expansion. The swap does nothing. A hygienic macro system renames the macro's `tmp`, so this cannot happen.

```c run id=functions
#include <stdio.h>

static inline int max_int(int a, int b) {
    return a > b ? a : b;
}

static inline void swap_int(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int main(void) {
    int i = 5;
    int m = max_int(i++, 0);
    printf("m = %d, i = %d\n", m, i);

    int x = 1;
    int tmp = 2;
    swap_int(&x, &tmp);
    printf("x = %d, tmp = %d\n", x, tmp);
}
```

```output for=functions
m = 5, i = 6
x = 2, tmp = 1
```

- Verdict: native. The preprocessor replaces each macro invocation with its replacement list before the compiler parses the program.
- A macro sees tokens, not expressions, so the body needs parentheses. It has no scope, so its names can clash with the names of the caller.
- A `static inline` function evaluates each argument once, has a type, and has its own scope. Use it unless the macro needs the text of its argument (`#`), an identifier made from parts (`##`) or the location of the call (`__FILE__`, `__LINE__`).
