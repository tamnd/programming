```c run id=points
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

struct point {
    int x;
    int y;
};

static bool point_equal(struct point a, struct point b) {
    return a.x == b.x && a.y == b.y;
}

int main(void) {
    struct point a = {1, 2};
    struct point b = {1, 2};
    struct point *pa = &a;
    struct point *pb = &b;
    struct point *pc = &a;
    printf("point_equal(a, b): %d\n", point_equal(a, b));
    printf("pa == pb: %d\n", pa == pb);
    printf("pa == pc: %d\n", pa == pc);

    char s[] = "hello";
    char t[] = "hello";
    printf("strcmp(s, t) == 0: %d\n", strcmp(s, t) == 0);

    double nan = NAN;
    printf("nan == nan: %d\n", nan == nan);
    return 0;
}
```

```output for=points
point_equal(a, b): 1
pa == pb: 0
pa == pc: 1
strcmp(s, t) == 0: 1
nan == nan: 0
```

- Verdict: idiom. `==` works on numbers and pointers only. Two pointers are equal when they point to the same object, so `==` on pointers is identity. A struct needs a comparison function that the program writes, field by field.
- Strings are arrays of `char`. `strcmp` compares the characters. `==` on two arrays compares their addresses.
- Do not compare structs with `memcmp`. The padding bytes of a struct have unspecified values, so two structs with equal fields can have different bytes.

```c fails expect=compile id=struct-eq
#include <stdio.h>

struct point {
    int x;
    int y;
};

int main(void) {
    struct point a = {1, 2};
    struct point b = {1, 2};
    printf("%d\n", a == b);
    return 0;
}
```

The operands of `==` must be numbers or pointers, so a compiler must reject a comparison of two structs.

```c wrong id=string-eq
#include <stdio.h>
#include <string.h>

int main(void) {
    char input[16];
    strcpy(input, "quit");
    const char *command = "quit";
    if (input == command) {
        printf("bye\n");
    } else {
        printf("unknown command: %s\n", input);
    }
    return 0;
}
```

```output for=string-eq
unknown command: quit
```

`input` is an array on the stack and `command` points to a string literal. They are different objects, so `==` is false although they hold the same characters. Use `strcmp(input, command) == 0`.
