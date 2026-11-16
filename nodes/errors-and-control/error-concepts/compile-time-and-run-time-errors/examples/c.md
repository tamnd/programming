```c run id=when-c
#include <stdio.h>

int main(void) {
    int items[] = {10, 20, 30};
    size_t n = sizeof items / sizeof items[0];
    size_t indexes[] = {1, 5};
    for (int k = 0; k < 2; k++) {
        size_t i = indexes[k];
        if (i < n) {
            printf("items[%zu] = %d\n", i, items[i]);
        } else {
            printf("items[%zu]: out of range (tested by the code)\n", i);
        }
    }
    return 0;
}
```

```output for=when-c
items[1] = 20
items[5]: out of range (tested by the code)
```

- Verdict: absent. The C compiler finds syntax and type errors, and the linker finds a function that has no definition. Nothing tests an array index at run time: an index past the end has undefined behavior, so the code must test it.
