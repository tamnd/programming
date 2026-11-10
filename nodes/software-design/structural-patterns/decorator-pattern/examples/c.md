```c run id=repository
#include <stdio.h>
#include <string.h>

/* The interface: a table of function pointers and a pointer to the object. */
typedef struct Repository Repository;
struct Repository {
    void (*get)(Repository *self, const char *key, char *out, size_t size);
    const char *(*name)(Repository *self);
};

static void store_get(Repository *self, const char *key, char *out, size_t size) {
    (void)self;
    printf("store: load %s\n", key);
    snprintf(out, size, "value-%s", key);
}

static const char *store_name(Repository *self) {
    (void)self;
    return "store";
}

/* The Repository member comes first, so a pointer to it is a pointer to the decorator. */
typedef struct {
    Repository base;
    Repository *inner;
} Logged;

static void logged_get(Repository *self, const char *key, char *out, size_t size) {
    Logged *logged = (Logged *)self;
    printf("log: get %s\n", key);
    logged->inner->get(logged->inner, key, out, size);
}

static const char *logged_name(Repository *self) {
    Logged *logged = (Logged *)self;
    return logged->inner->name(logged->inner);
}

enum { SLOTS = 8, KEY_SIZE = 16, VALUE_SIZE = 32 };

typedef struct {
    Repository base;
    Repository *inner;
    int count;
    char keys[SLOTS][KEY_SIZE];
    char values[SLOTS][VALUE_SIZE];
} Cached;

static void cached_get(Repository *self, const char *key, char *out, size_t size) {
    Cached *cached = (Cached *)self;
    for (int i = 0; i < cached->count; i++) {
        if (strcmp(cached->keys[i], key) == 0) {
            snprintf(out, size, "%s", cached->values[i]);
            return;
        }
    }
    cached->inner->get(cached->inner, key, out, size);
    if (cached->count < SLOTS) {
        snprintf(cached->keys[cached->count], KEY_SIZE, "%s", key);
        snprintf(cached->values[cached->count], VALUE_SIZE, "%s", out);
        cached->count++;
    }
}

static const char *cached_name(Repository *self) {
    Cached *cached = (Cached *)self;
    return cached->inner->name(cached->inner);
}

int main(void) {
    Repository store = {store_get, store_name};
    Cached cached = {{cached_get, cached_name}, &store, 0, {{0}}, {{0}}};
    Logged logged = {{logged_get, logged_name}, &cached.base};
    Repository *repo = &logged.base;

    const char *keys[] = {"a", "a", "b"};
    char value[VALUE_SIZE];
    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; i++) {
        repo->get(repo, keys[i], value, sizeof value);
        printf("%s\n", value);
    }
    printf("name: %s\n", repo->name(repo));
    return 0;
}
```

```output
log: get a
store: load a
value-a
log: get a
value-a
log: get b
store: load b
value-b
name: store
```

- Verdict: idiom. C has no interfaces, so the interface is a struct of function pointers. Each decorator puts that struct first and adds a pointer to the inner object.
- The cast from `Repository *` to `Logged *` is valid. A pointer to a structure, suitably converted, points to its first member, and the reverse also holds.
- The compiler does not check that each decorator fills every function pointer. A missing one is a null pointer, and the call crashes at run time.
- The objects live on the stack of `main`, so the program does not allocate. A chain built at run time needs `malloc` and a matching function to free it.
