```c run id=precedence-climbing
#include <stdio.h>
#include <stdlib.h>

/* Precedence climbing: a table of levels replaces one grammar rule per level. */
static const char *p;

static int prec(char op) {
    switch (op) {
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    case '^': return 3;
    default: return 0;
    }
}

static void skip(void) {
    while (*p == ' ') p++;
}

static long expr(int min);

static long atom(void) {
    skip();
    if (*p == '(') {
        p++;
        long v = expr(1);
        skip();
        p++; /* the closing parenthesis */
        return v;
    }
    char *end;
    long v = strtol(p, &end, 10);
    p = end;
    return v;
}

static long power(long b, long e) {
    long r = 1;
    while (e-- > 0) r *= b;
    return r;
}

static long expr(int min) {
    long left = atom();
    for (;;) {
        skip();
        char op = *p;
        int q = prec(op);
        if (q == 0 || q < min) return left;
        p++;
        long right = expr(op == '^' ? q : q + 1); /* '^' groups to the right */
        switch (op) {
        case '+': left += right; break;
        case '-': left -= right; break;
        case '*': left *= right; break;
        case '/': left /= right; break;
        default: left = power(left, right); break;
        }
    }
}

int main(void) {
    const char *tests[] = {"10 - 3 - 2", "1 + 2 * 3", "2 ^ 3 ^ 2", "(2 ^ 3) ^ 2"};
    for (int i = 0; i < 4; i++) {
        p = tests[i];
        printf("%-12s = %ld\n", tests[i], expr(1));
    }
    return 0;
}
```

```output for=precedence-climbing
10 - 3 - 2   = 5
1 + 2 * 3    = 7
2 ^ 3 ^ 2    = 512
(2 ^ 3) ^ 2  = 64
```

```c wrong id=dangling-else
#include <stdio.h>

static void check(int a, int b) {
    printf("a=%2d b=%2d:", a, b);
    if (a > 0)
        if (b > 0)
            printf(" both positive");
    else /* the indentation suggests the outer if */
        printf(" a is not positive");
    printf("\n");
}

int main(void) {
    check(1, 1);
    check(1, -1);
    check(-1, 1);
    return 0;
}
```

```output for=dangling-else
a= 1 b= 1: both positive
a= 1 b=-1: a is not positive
a=-1 b= 1:
```

- Verdict: library. yacc and its successor GNU Bison generate C parsers from grammars, and lex and flex generate the lexers that feed them. The run block parses with precedence climbing, which replaces one grammar rule for each level with a table of levels. Operators on the left group first, except `^`, which groups to the right, so `2 ^ 3 ^ 2` is `2 ^ 9`.
- The wrong block shows the dangling else. The grammar of C allows both readings of `if (a) if (b) x; else y;`, and the standard picks the inner `if`, whatever the indentation shows. So the second call prints the message for the wrong case, and the third prints nothing. Use braces on every branch of a nested `if`.
