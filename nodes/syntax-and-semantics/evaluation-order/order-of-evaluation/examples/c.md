```c run id=sequenced
#include <stdio.h>

static int f(const char *name, int value) {
    printf("evaluate %s\n", name);
    return value;
}

int main(void) {
    int both = f("left", 0) && f("right", 1);
    int last = (f("first", 1), f("second", 2));
    int pick = f("test", 1) ? f("then", 3) : f("else", 4);
    printf("%d %d %d\n", both, last, pick);
    return 0;
}
```

```output for=sequenced
evaluate left
evaluate first
evaluate second
evaluate test
evaluate then
0 2 3
```

- Verdict: absent. Except for a few operators, the order in which C evaluates subexpressions is unspecified. The standard says that the side effects and value computations of subexpressions are unsequenced.
- `&&`, `||`, `?:` and the comma operator have a sequence point after their first operand. Only these operators give a fixed order. `&&` stops after `left` because the result is already false.
- The arguments of a call can be evaluated in any order. There is a sequence point after the arguments and before the call.

```c fails expect=compile flags="-Werror=sequence-point" id=unsequenced
#include <stdio.h>

int main(void) {
    int i = 1;
    int j = i++ + i++;
    printf("%d\n", j);
    return 0;
}
```

Two changes to `i` with no sequence point between them are undefined behavior, not only an unspecified order. GCC reports this with `-Wsequence-point` and Clang reports it with `-Wunsequenced`. Both are in `-Wall`, and Clang accepts the GCC name. The flag makes the warning an error.
