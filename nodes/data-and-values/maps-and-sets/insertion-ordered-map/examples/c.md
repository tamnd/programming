```c run id=linked-hash-table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A hash table whose entries are also on a doubly linked list in insertion
   order. This is the design of Java's LinkedHashMap. */
#define NB 8

struct entry {
    char key[16];
    int val;
    struct entry *bnext;          /* next entry in the same bucket */
    struct entry *prev, *next;    /* insertion-order list */
};

static struct entry *buckets[NB];
static struct entry *head, *tail;

static unsigned hash(const char *s) {
    unsigned h = 2166136261u;
    for (; *s; s++) h = (h ^ (unsigned char)*s) * 16777619u;
    return h % NB;
}

static struct entry *find(const char *k) {
    for (struct entry *e = buckets[hash(k)]; e; e = e->bnext)
        if (strcmp(e->key, k) == 0) return e;
    return NULL;
}

static void put(const char *k, int v) {
    struct entry *e = find(k);
    if (e) { e->val = v; return; }    /* an update keeps its place */
    e = calloc(1, sizeof *e);
    snprintf(e->key, sizeof e->key, "%s", k);
    e->val = v;
    unsigned b = hash(k);
    e->bnext = buckets[b];
    buckets[b] = e;
    e->prev = tail;                   /* append to the order list */
    if (tail) tail->next = e; else head = e;
    tail = e;
}

static void del(const char *k) {
    unsigned b = hash(k);
    for (struct entry **p = &buckets[b]; *p; p = &(*p)->bnext) {
        struct entry *e = *p;
        if (strcmp(e->key, k) != 0) continue;
        *p = e->bnext;
        /* unlink from the order list in constant time */
        if (e->prev) e->prev->next = e->next; else head = e->next;
        if (e->next) e->next->prev = e->prev; else tail = e->prev;
        free(e);
        return;
    }
}

static void show(void) {
    for (struct entry *e = head; e; e = e->next)
        printf("%s=%d%s", e->key, e->val, e->next ? " " : "\n");
}

int main(void) {
    put("pear", 4);
    put("apple", 7);
    put("fig", 2);
    show();
    put("pear", 9);
    show();
    del("pear");
    put("pear", 1);
    show();
    printf("first %s, last %s\n", head->key, tail->key);
    while (head) del(head->key);
    printf("empty %d\n", head == NULL && tail == NULL);
    return 0;
}
```

```output
pear=4 apple=7 fig=2
pear=9 apple=7 fig=2
apple=7 fig=2 pear=1
first apple, last pear
empty 1
```

- Verdict: idiom. C has no maps. The example is a chained hash table whose entries are also on a doubly linked list in insertion order, which is the design of Java LinkedHashMap. An update keeps the place, and a delete unlinks the entry in constant time.
