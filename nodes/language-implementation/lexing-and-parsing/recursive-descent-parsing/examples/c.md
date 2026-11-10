```c run id=parser
#include <ctype.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>

/* The parser state is global: the source, the cursor and the jump target for errors. */
static const char *src;
static size_t pos;
static jmp_buf on_error;
static char message[80];

typedef struct {
    char text[128];
    long value;
} Result;

static void fail(const char *expected) {
    snprintf(message, sizeof message, "expected %s at column %zu", expected, pos + 1);
    longjmp(on_error, 1);
}

/* The lexer works on demand: peek skips spaces and returns the next token kind. */
static char peek(void) {
    while (src[pos] == ' ') pos++;
    char c = src[pos];
    if (c == '\0') return 'e';
    if (isdigit((unsigned char)c)) return 'n';
    if (strchr("+-*/^()", c)) return c;
    snprintf(message, sizeof message, "unexpected character '%c' at column %zu", c, pos + 1);
    longjmp(on_error, 1);
}

static Result combine(char op, Result l, Result r) {
    Result out;
    long a = l.value, b = r.value, v = 1;
    switch (op) {
    case '+': v = a + b; break;
    case '-': v = a - b; break;
    case '*': v = a * b; break;
    case '/': v = a / b; break;
    default: for (long k = 0; k < b; k++) v *= a;
    }
    snprintf(out.text, sizeof out.text, "(%s %c %s)", l.text, op, r.text);
    out.value = v;
    return out;
}

static Result expr(void);
static Result unary(void);

static Result atom(void) {
    Result out;
    char k = peek();
    if (k == 'n') {
        size_t start = pos;
        out.value = 0;
        while (isdigit((unsigned char)src[pos])) out.value = out.value * 10 + (src[pos++] - '0');
        snprintf(out.text, sizeof out.text, "%.*s", (int)(pos - start), src + start);
        return out;
    }
    if (k == '(') {
        pos++;
        out = expr();
        if (peek() != ')') fail("')'");
        pos++;
        return out;
    }
    fail("a number or '('");
    return out;
}

static Result power(void) {
    Result base = atom();
    if (peek() != '^') return base;
    pos++;
    return combine('^', base, unary());
}

static Result unary(void) {
    if (peek() != '-') return power();
    pos++;
    Result r = unary(), out;
    snprintf(out.text, sizeof out.text, "(-%s)", r.text);
    out.value = -r.value;
    return out;
}

static Result term(void) {
    Result left = unary();
    for (char op = peek(); op == '*' || op == '/'; op = peek()) {
        pos++;
        left = combine(op, left, unary());
    }
    return left;
}

static Result expr(void) {
    Result left = term();
    for (char op = peek(); op == '+' || op == '-'; op = peek()) {
        pos++;
        left = combine(op, left, term());
    }
    return left;
}

int main(void) {
    const char *inputs[] = {"1 + 2 * 3", "1 - 2 - 3", "2 ^ 3 ^ 2", "-2 ^ 2", "(1 + 2) * 3", "1 + * 2", "(1 + 2", "1 $ 2"};
    for (size_t n = 0; n < sizeof inputs / sizeof inputs[0]; n++) {
        src = inputs[n];
        pos = 0;
        if (setjmp(on_error) != 0) {
            printf("%s -> error: %s\n", src, message);
            continue;
        }
        Result r = expr();
        if (peek() != 'e') fail("an operator");
        printf("%s -> %s = %ld\n", src, r.text, r.value);
    }
    return 0;
}
```

```output for=parser
1 + 2 * 3 -> (1 + (2 * 3)) = 7
1 - 2 - 3 -> ((1 - 2) - 3) = -4
2 ^ 3 ^ 2 -> (2 ^ (3 ^ 2)) = 512
-2 ^ 2 -> (-(2 ^ 2)) = -4
(1 + 2) * 3 -> ((1 + 2) * 3) = 9
1 + * 2 -> error: expected a number or '(' at column 5
(1 + 2 -> error: expected ')' at column 7
1 $ 2 -> error: unexpected character '$' at column 3
```

- Verdict: idiom. One function per rule over a global cursor. The lexer is not a separate pass: `peek` skips spaces and classifies the next character when the parser asks for it.
- An error calls `longjmp`, which returns control to the `setjmp` in `main` and discards every frame of the recursion at once. This is safe here because the parser allocates no memory on the heap. A parser that called `malloc` would leak whatever it held at the time of the jump.
- The text of each result lives in a fixed buffer of 128 bytes, and `snprintf` cuts off longer text. A real parser would build an [[abstract-syntax-tree]] on the heap instead.
