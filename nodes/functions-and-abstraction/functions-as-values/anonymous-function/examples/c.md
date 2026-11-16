```c run id=anonymous-function-c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int by_length(const void *a, const void *b) {
    size_t la = strlen(*(const char *const *)a);
    size_t lb = strlen(*(const char *const *)b);
    return (la > lb) - (la < lb);
}

static int square(int x) { return x * x; }

static void map(int (*f)(int), int *xs, size_t n) {
    for (size_t i = 0; i < n; i++) {
        xs[i] = f(xs[i]);
    }
}

struct adder {
    int n;
};

static int adder_call(const struct adder *a, int x) { return x + a->n; }

int main(void) {
    const char *words[] = {"house", "car", "bike"};
    qsort(words, 3, sizeof words[0], by_length);
    printf("%s %s %s\n", words[0], words[1], words[2]);

    int nums[] = {1, 2, 3};
    map(square, nums, 3);
    printf("%d %d %d\n", nums[0], nums[1], nums[2]);

    struct adder add = {5};
    printf("%d\n", adder_call(&add, 10));
    return 0;
}
```

```output for=anonymous-function-c
car bike house
1 4 9
15
```

- Verdict: absent. C has no anonymous functions. The program defines named functions and passes pointers to them. The adder is a struct that holds `n` and a function that takes the struct.
