```c run id=linkage-and-block-scope
#include <stdio.h>

/* static at file scope gives internal linkage: the name is bound only in this file.
   A name with external linkage is bound by the linker to one object in the program. */
static int calls = 0;

int next_id(void) {
    static int id = 100; /* one object for all calls, initialized once */
    calls++;
    return id++;
}

int main(void) {
    int x = 1;
    {
        int x = 2; /* a new x in the inner block hides the outer x */
        x++;
        printf("inner %d\n", x);
    }
    printf("outer %d\n", x);

    /* A pointer gives a second way to reach an object. */
    int *p = &x;
    *p = 7;
    printf("x %d\n", x);

    next_id();
    next_id();
    printf("%d after %d calls\n", next_id(), calls);
    return 0;
}
```

```output for=linkage-and-block-scope
inner 3
outer 1
x 7
102 after 3 calls
```

- Verdict: native. `static` at file scope gives a name internal linkage, and the linker binds a name with external linkage to one object in the program. An inner block can declare a new `x` that hides the outer one. A `static` local is one object for all calls. A pointer is a second way to reach an object.
