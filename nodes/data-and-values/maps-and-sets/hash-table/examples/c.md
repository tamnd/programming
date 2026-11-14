```c run id=chained-table
#define _POSIX_C_SOURCE 200809L /* for strdup before C23 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A hash table with separate chaining and string keys. */
typedef struct entry {
    char *key;
    int value;
    struct entry *next;
} entry;

typedef struct {
    entry **buckets;
    size_t size, count;
} table;

/* FNV-1a: a simple 32-bit string hash. */
static unsigned fnv1a(const char *s) {
    unsigned h = 2166136261u;
    for (; *s; s++) {
        h ^= (unsigned char)*s;
        h *= 16777619u;
    }
    return h;
}

static void init(table *t, size_t size) {
    t->buckets = calloc(size, sizeof *t->buckets);
    t->size = size;
    t->count = 0;
}

static void grow(table *t) {
    table bigger;
    init(&bigger, t->size * 2);
    for (size_t i = 0; i < t->size; i++) {
        entry *e = t->buckets[i];
        while (e) {
            entry *next = e->next;
            size_t j = fnv1a(e->key) % bigger.size;
            e->next = bigger.buckets[j];
            bigger.buckets[j] = e;
            e = next;
        }
    }
    free(t->buckets);
    bigger.count = t->count;
    *t = bigger;
}

static void put(table *t, const char *key, int value) {
    size_t i = fnv1a(key) % t->size;
    for (entry *e = t->buckets[i]; e; e = e->next) {
        if (strcmp(e->key, key) == 0) {
            e->value = value;
            return;
        }
    }
    entry *e = malloc(sizeof *e);
    e->key = strdup(key);
    e->value = value;
    e->next = t->buckets[i];
    t->buckets[i] = e;
    /* Keep the load factor count / size at 0.75 or less. */
    if (++t->count * 4 > t->size * 3) grow(t);
}

static int *get(table *t, const char *key) {
    for (entry *e = t->buckets[fnv1a(key) % t->size]; e; e = e->next)
        if (strcmp(e->key, key) == 0) return &e->value;
    return NULL;
}

static void show(const table *t) {
    printf("%zu buckets:", t->size);
    for (size_t i = 0; i < t->size; i++) {
        size_t n = 0;
        for (entry *e = t->buckets[i]; e; e = e->next) n++;
        printf(" %zu", n);
    }
    printf("\n");
}

int main(void) {
    const char *words[] = {"apple", "banana", "cherry", "date", "elder", "fig", "grape"};
    table t;
    init(&t, 8);
    for (int i = 0; i < 6; i++) put(&t, words[i], i);
    show(&t);
    put(&t, "grape", 6); /* 7 > 0.75 * 8, so the table doubles */
    show(&t);
    put(&t, "fig", 50); /* an update, not a new entry */
    int *v = get(&t, "fig");
    printf("%d %zu %s\n", v ? *v : -1, t.count, get(&t, "kiwi") ? "found" : "missing");
    for (size_t i = 0; i < t.size; i++) {
        entry *e = t.buckets[i];
        while (e) {
            entry *next = e->next;
            free(e->key);
            free(e);
            e = next;
        }
    }
    free(t.buckets);
    return 0;
}
```

```output
8 buckets: 2 1 0 0 0 1 0 2
16 buckets: 2 0 0 0 0 1 0 1 1 1 0 0 0 0 0 1
50 7 missing
```

- Verdict: idiom. C has no hash table in its standard library. This example writes one with separate chaining, the FNV-1a hash and a table that doubles when the load factor passes 0.75. POSIX hsearch gives one table for each program.
