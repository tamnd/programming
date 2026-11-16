```c run id=void-pointer
#include <stdio.h>
#include <string.h>

enum kind { INT, TEXT };

/* A void * can point to data of any type, but the type is not stored with it. */
struct box {
    enum kind kind;
    void *data;
};

static void show(struct box b) {
    if (b.kind == INT) {
        printf("int %d\n", *(int *)b.data);
    } else {
        printf("text %s\n", (char *)b.data);
    }
}

int main(void) {
    int n = 42;
    char text[] = "hi";
    struct box items[] = {{INT, &n}, {TEXT, text}};
    for (size_t i = 0; i < sizeof items / sizeof items[0]; i++) {
        show(items[i]);
    }

    /* memcpy takes void * parameters, so it works on objects of any type. */
    double a = 2.5;
    double b = 0;
    memcpy(&b, &a, sizeof a);
    printf("%.1f\n", b);
    return 0;
}
```

```output for=void-pointer
int 42
text hi
2.5
```

- Verdict: idiom. C has no top type, but a `void *` can point to an object of any type. The pointer does not record that type, so `struct box` stores a tag next to it, and `show` casts the pointer back by the tag. A wrong tag would make the cast wrong, and C would not detect it. Library functions such as `memcpy` take `void *` so that they work on any object.
