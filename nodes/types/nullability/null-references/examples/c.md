```c run id=null
#include <stdio.h>
#include <string.h>

struct user {
    const char *name;
    int age;
};

static const struct user *find(const struct user *users, size_t n, const char *name) {
    for (size_t i = 0; i < n; i++) {
        if (strcmp(users[i].name, name) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

int main(void) {
    struct user users[] = {{"ada", 36}, {"alan", 41}};
    const char *names[] = {"alan", "bob"};
    for (size_t i = 0; i < 2; i++) {
        const struct user *u = find(users, 2, names[i]);
        if (u != NULL) {
            printf("%s: %d\n", u->name, u->age);
        } else {
            printf("%s: not found\n", names[i]);
        }
    }
    return 0;
}
```

```output
alan: 41
bob: not found
```

- Verdict: native. Every pointer type has a null pointer value. The constant `0`, or `0` cast to `void *`, converts to it, and the macro `NULL` expands to such a constant.
- A null pointer compares unequal to a pointer to any object or function, so `find` can return it to mean "no match". Any two null pointers compare equal.
- The unary `*` operator, and so `->`, on a null pointer is undefined behaviour. The language does not check. A run with AddressSanitizer shows the fault (see How it works on the concept page).
