```c run id=lvalues-and-struct-copy
#include <stdio.h>

struct point {
    int x, y;
};

int main(void) {
    /* x is an lvalue: it has an address and can be assigned. x + 1 is not an lvalue. */
    int x = 5;
    int *p = &x;
    *p = 6;
    printf("%d\n", x);

    /* A const object is an lvalue, but not a modifiable lvalue. */
    const int limit = 10;
    printf("%d %d\n", limit, *&limit);

    /* Assignment of a struct copies the value of all its members. */
    struct point a = {1, 2};
    struct point b = a;
    b.x = 10;
    printf("%d %d\n", a.x, b.x);

    /* A compound literal makes an unnamed object, which is an lvalue. */
    int *q = (int[]){7, 8, 9};
    q[0] = 70;
    printf("%d %d\n", q[0], q[2]);

    /* An array is not a modifiable lvalue, so it cannot be assigned, but its elements can. */
    int arr[3] = {0};
    arr[1] = 4;
    printf("%d %zu\n", arr[1], sizeof arr / sizeof arr[0]);
    return 0;
}
```

```output for=lvalues-and-struct-copy
6
10 10
1 10
70 9
4 3
```

- Verdict: native. `x` is an lvalue, so `&x` gives its address and `*p = 6` changes it. A `const` object is an lvalue but not a modifiable lvalue. Assignment copies all members of a struct. A compound literal makes an unnamed object that is an lvalue. An array cannot be assigned, but its elements can.
