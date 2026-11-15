```c run id=for-while-do-and-goto
#include <stdio.h>

int main(void) {
    int sum = 0;
    for (int i = 1; i <= 5; i++) {
        sum += i;
    }
    printf("for: %d\n", sum);

    int n = 100, steps = 0;
    while (n > 1) {
        n /= 2;
        steps++;
    }
    printf("while: %d\n", steps);

    /* The do body runs before the first test. */
    int runs = 0;
    do {
        runs++;
    } while (runs < 0);
    printf("do: %d\n", runs);

    /* Before structured loops, a loop was a backward jump. */
    int k = 0;
again:
    k++;
    if (k < 3) {
        goto again;
    }
    printf("goto: %d\n", k);
    return 0;
}
```

```output for=for-while-do-and-goto
for: 15
while: 6
do: 1
goto: 3
```

- Verdict: native. C has `for`, `while` and `do`. The `do` body runs once although the condition is false. The last loop is a label and a `goto` that jumps back, the form of a loop in assembly language and in early Fortran. The structured loops do the same job and are easier to read.
