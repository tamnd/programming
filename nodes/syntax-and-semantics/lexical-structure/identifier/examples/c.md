```c run id=name-spaces-and-func
#include <stdio.h>

#define NAME_OF(x) #x

/* C has separate name spaces: a struct tag, a label and an ordinary name do not clash. */
struct point { int x, y; };

int main(void) {
    struct point point = {1, 2};
    int n = 3, N = 4; /* names are case-sensitive */
    printf("%d %d %d\n", point.x + point.y, n, N);

    /* C23 allows names with letters of other scripts. */
    int größe = 5;
    printf("%d\n", größe);

    /* __func__ is a predefined name that holds the name of the function. */
    printf("%s\n", __func__);

    /* The preprocessor can turn a name into a string. */
    printf("%s\n", NAME_OF(point));

    /* Names that start with __, or with _ and an upper-case letter, are reserved for
       the implementation. A program that declares one has undefined behavior. */
    goto point;
point:
    printf("label point\n");
    return 0;
}
```

```output for=name-spaces-and-func
3 3 4
5
main
point
label point
```

- Verdict: native. C keeps struct tags, labels and ordinary names in separate name spaces, so `point` names a tag, a variable and a label at once. Names are case-sensitive. C23 accepts names with letters of other scripts. `__func__` holds the name of the function, and the `#` operator of the preprocessor turns a name into a string. Names that start with `__`, or with `_` and an upper-case letter, are reserved.
