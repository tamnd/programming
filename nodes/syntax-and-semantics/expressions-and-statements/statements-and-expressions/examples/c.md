```c run id=assignment-and-comma-expressions
#include <stdio.h>

static const char *label(int score) {
    return score >= 90 ? "A" : score >= 50 ? "pass" : "fail";
}

int main(void) {
    int scores[] = {95, 70, 30};
    for (int k = 0; k < 3; k++) printf(k ? " %s" : "%s", label(scores[k]));
    printf("\n");

    int a, b;
    a = b = 5; /* assignment is an expression */
    printf("%d %d\n", a, b);

    int data[] = {3, 1, 4, 0, 5};
    int i = 0, n, total = 0;
    while ((n = data[i++]) != 0) total += n;
    printf("%d\n", total);

    int c = (a++, a * 2); /* the comma operator: the value of the right side */
    printf("%d %d\n", a, c);
    return 0;
}
```

```output for=assignment-and-comma-expressions
A pass fail
5 5
8
6 12
```

```c wrong id=assignment-in-condition
#include <stdio.h>

int main(void) {
    int errors = 3;
    if (errors = 0) { /* meant ==; this assigns 0, and 0 is false */
        printf("no errors\n");
    } else {
        printf("errors: %d\n", errors);
    }
    return 0;
}
```

```output for=assignment-in-condition
errors: 0
```

- Verdict: native. In C, assignment is an expression, and any expression followed by a semicolon is a statement. This lets the loop read and test in one condition, and lets `a = b = 5` assign twice. The comma operator evaluates its left side, then gives the value of its right side, so `c` is 12 after `a++` makes `a` 6.
- The wrong block writes `=` where it means `==`. The condition assigns 0 to `errors`, and 0 is false, so the program takes the else branch and prints 0 errors where there were 3. Compilers warn about an assignment used as a condition with `-Wall`; extra parentheses tell them it is meant.
