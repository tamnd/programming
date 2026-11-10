```c run id=tree
#include <stddef.h>
#include <stdio.h>

struct node {
    int value;
    struct node *left;
    struct node *right;
};

static int total(const struct node *tree) {
    if (tree == NULL) {
        return 0;
    }
    return tree->value + total(tree->left) + total(tree->right);
}

int main(void) {
    struct node four = {4, NULL, NULL};
    struct node five = {5, NULL, NULL};
    struct node two = {2, &four, &five};
    struct node three = {3, NULL, NULL};
    struct node one = {1, &two, &three};
    printf("%d\n", total(&one));
    return 0;
}
```

```output for=tree
15
```

- Verdict: native. A C function can call itself, and each call gets an activation record on the stack.
- C does not check the depth of recursion. When the stack is full, the process usually gets a signal and stops (see the last block).
- The standard does not require tail call elimination. GCC turns sibling and tail recursive calls into jumps at `-O2`, `-O3` and `-Os`. The program below counts to 100,000,000 with a tail call at `-O2`.
- Clang's attribute `musttail` on a `return` statement requires the compiler to emit a tail call, even without optimization.

```c run flags="-O2" id=tail
#include <stdio.h>

static long count(long n, long steps) {
    if (n == 0) {
        return steps;
    }
    return count(n - 1, steps + 1);
}

int main(void) {
    printf("%ld\n", count(100000000, 0));
    return 0;
}
```

```output for=tail
100000000
```

A deep recursion that is not a tail call overflows the stack. Under AddressSanitizer, the run stops with a `stack-overflow` report. Without the sanitizer, the process usually dies with a segmentation fault.

```c fails expect=sanitizer:address id=overflow
#include <stdio.h>

static long depth(long n) {
    volatile long frame = n;
    if (n == 0) {
        return 0;
    }
    return depth(n - 1) + (frame > 0);
}

int main(void) {
    printf("%ld\n", depth(100000000));
    return 0;
}
```
