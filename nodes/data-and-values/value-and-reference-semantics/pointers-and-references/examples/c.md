```c run id=change
#include <stdio.h>

static void set_copy(int n) {
    n = 42;
    printf("inside set_copy: %d\n", n);
}

static void set_pointer(int *p) { *p = 42; }

static void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int main(void) {
    int x = 1;
    set_copy(x);
    printf("after set_copy: %d\n", x);
    set_pointer(&x);
    printf("after set_pointer: %d\n", x);

    int a = 1;
    int b = 2;
    swap(&a, &b);
    printf("after swap: a=%d b=%d\n", a, b);

    int values[3] = {10, 20, 30};
    int *p = values;
    printf("p[1] == *(p + 1): %d\n", p[1] == *(p + 1));
    printf("p + 3 == &values[3]: %d\n", p + 3 == &values[3]);
    printf("sizeof(int *): %zu, alignof(int): %zu\n", sizeof(int *), alignof(int));
    return 0;
}
```

```output for=change
inside set_copy: 42
after set_copy: 1
after set_pointer: 42
after swap: a=2 b=1
p[1] == *(p + 1): 1
p + 3 == &values[3]: 1
sizeof(int *): 8, alignof(int): 4
```

- Verdict: native. A pointer type `T *` holds the address of an object of type `T`. The operator `&` gives the address, and `*` reads or writes the object.
- C passes every argument by value. A function changes a variable of the caller only through a pointer, as in `set_pointer` and `swap`.
- `p[i]` is defined as `*(p + i)`. Pointer arithmetic moves by whole elements, and a pointer may point one past the last element.
- The size of a pointer and the alignment of a type depend on the implementation.
