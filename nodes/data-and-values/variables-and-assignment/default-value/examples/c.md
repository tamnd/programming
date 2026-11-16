```c run id=zero-defaults
#include <stdio.h>
#include <stdlib.h>

struct config {
    int width;
    double scale;
    const char *name;
};

static struct config global;

int main(void) {
    printf("%d %.1f %s\n", global.width, global.scale, global.name == NULL ? "NULL" : global.name);
    int *counts = calloc(4, sizeof *counts);
    if (counts == NULL) {
        return 1;
    }
    printf("%d %d\n", counts[0], counts[3]);
    free(counts);
    struct config local = {0};
    printf("%d %.1f\n", local.width, local.scale);
    return 0;
}
```

```output for=zero-defaults
0 0.0 NULL
0 0
0 0.0
```

- Verdict: native. A static object without an initializer is zero: numbers are 0 and pointers are null. `calloc` gives memory set to zero, but `malloc` does not. An automatic variable has no default value, and `= {0}` sets all the members of a local struct to zero.
