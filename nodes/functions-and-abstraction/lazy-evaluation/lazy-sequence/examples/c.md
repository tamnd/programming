```c run id=pipeline
#include <stdbool.h>
#include <stdio.h>

struct odd_squares {
    int next_n;
};

static bool next_odd_square(struct odd_squares *it, int *out) {
    for (;;) {
        int n = it->next_n++;
        printf("square %d\n", n);
        int s = n * n;
        if (s % 2 == 1) {
            *out = s;
            return true;
        }
    }
}

int main(void) {
    struct odd_squares it = {.next_n = 1};
    int first[2];
    for (int i = 0; i < 2; i++) {
        if (!next_odd_square(&it, &first[i])) {
            return 1;
        }
    }
    printf("[%d, %d]\n", first[0], first[1]);
    return 0;
}
```

```output for=pipeline
square 1
square 2
square 3
[1, 9]
```

- Verdict: absent. C has no iterator protocol and no closures. A lazy sequence is a struct that holds the state and a `next` function that computes one element per call. Each stage of a pipeline is code inside `next`, or another struct that holds a pointer to the stage before it.
