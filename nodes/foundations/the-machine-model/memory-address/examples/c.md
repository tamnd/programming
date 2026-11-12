```c run id=pointer-arithmetic-and-offsetof
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

struct rec {
    char flag;
    int32_t count;
    char tag;
};

int main(void) {
    int32_t xs[4] = {10, 20, 30, 40};
    int32_t *p = &xs[0];        /* &: the address of an object */
    int32_t *q = p + 2;         /* pointer arithmetic counts elements, not bytes */
    printf("q - p = %td elements\n", q - p);
    printf("bytes apart: %td\n", (char *)q - (char *)p);
    printf("*q = %d, p[3] = %d\n", *q, p[3]); /* *: the object at an address */

    *q = 99;                    /* write through the address */
    printf("xs[2] = %d\n", xs[2]);

    printf("offsets: %zu %zu %zu, size %zu, align %zu\n",
           offsetof(struct rec, flag), offsetof(struct rec, count),
           offsetof(struct rec, tag), sizeof(struct rec), _Alignof(struct rec));
    printf("p aligned to 4: %d\n", (uintptr_t)p % 4 == 0);
    return 0;
}
```

```output for=pointer-arithmetic-and-offsetof
q - p = 2 elements
bytes apart: 8
*q = 30, p[3] = 40
xs[2] = 99
offsets: 0 4 8, size 12, align 4
p aligned to 4: 1
```

- Verdict: native. `&` takes the address of an object and `*` follows one. The difference of two pointers counts elements, and the difference of the same addresses as `char *` counts bytes. `p[3]` is `*(p + 3)`, and a write through a pointer changes the array. `offsetof`, `sizeof` and `_Alignof` give the layout, and a cast to `uintptr_t` gives the address as an integer to check the alignment.
