```c run id=array-of-pairs
#include <stdio.h>
#include <string.h>

// C has no map type. This map is an array of pairs with a linear search.
struct pair {
    const char *key;
    int value;
};

struct map {
    struct pair items[16];
    size_t len;
};

// find returns the index of key, or -1 if the key is not there.
static long find(const struct map *m, const char *key) {
    for (size_t i = 0; i < m->len; i++) {
        if (strcmp(m->items[i].key, key) == 0) {
            return (long)i;
        }
    }
    return -1;
}

// put updates the value of key, or appends a new pair. It returns 0, or -1 if the map is full.
static int put(struct map *m, const char *key, int value) {
    long i = find(m, key);
    if (i >= 0) {
        m->items[i].value = value;
        return 0;
    }
    if (m->len == sizeof m->items / sizeof m->items[0]) {
        return -1;
    }
    m->items[m->len++] = (struct pair){key, value};
    return 0;
}

// get writes the value to out and returns 1, or returns 0 if the key is not there.
static int get(const struct map *m, const char *key, int *out) {
    long i = find(m, key);
    if (i < 0) {
        return 0;
    }
    *out = m->items[i].value;
    return 1;
}

// del moves the last pair into the hole, so the order of the pairs changes.
static void del(struct map *m, const char *key) {
    long i = find(m, key);
    if (i >= 0) {
        m->items[i] = m->items[--m->len];
    }
}

int main(void) {
    struct map m = {.len = 0};
    put(&m, "x", 1);
    put(&m, "y", 2);
    put(&m, "z", 3);
    put(&m, "x", 10);
    int v = 0;
    int found = get(&m, "x", &v);
    printf("%d %d %zu\n", found, v, m.len);
    found = get(&m, "w", &v);
    printf("%d\n", found);
    del(&m, "x");
    for (size_t i = 0; i < m.len; i++) {
        printf("%s=%d\n", m.items[i].key, m.items[i].value);
    }
    return 0;
}
```

```output
1 10 3
0
z=3
y=2
```

- Verdict: idiom. C has no map type. This example keeps an array of key and value pairs with a linear search, so each operation takes linear time. A real program uses a hash table library, or hcreate and hsearch from POSIX, which take no table argument, so a program has one such table at a time.
