```c run id=slot-table
#include <stdio.h>
#include <string.h>

/* An object: a parent pointer and a small table of named slots. */
struct obj;
typedef void (*method)(const struct obj *self);

struct slot {
    const char *name;
    int value;
    method fn;
};

struct obj {
    const struct obj *parent;
    int n;
    struct slot slots[4];
};

static const struct slot *lookup(const struct obj *o, const char *name)
{
    for (; o != NULL; o = o->parent) /* walk the delegation chain */
        for (int i = 0; i < o->n; i++)
            if (strcmp(o->slots[i].name, name) == 0)
                return &o->slots[i];
    return NULL;
}

static void set(struct obj *o, const char *name, int value, method fn)
{
    for (int i = 0; i < o->n; i++)
        if (strcmp(o->slots[i].name, name) == 0) {
            o->slots[i].value = value;
            o->slots[i].fn = fn;
            return;
        }
    o->slots[o->n++] = (struct slot){name, value, fn};
}

static int get(const struct obj *o, const char *name) { return lookup(o, name)->value; }
static void send(const struct obj *o, const char *name) { lookup(o, name)->fn(o); }

static void describe_round(const struct obj *s) { printf("(%d, %d)", get(s, "x"), get(s, "y")); }
static void describe_angle(const struct obj *s) { printf("<%d %d>", get(s, "x"), get(s, "y")); }

int main(void)
{
    struct obj traits = {0}, origin = {.parent = &traits}, p = {.parent = &origin};
    set(&traits, "describe", 0, describe_round);
    set(&origin, "x", 0, NULL);
    set(&origin, "y", 0, NULL);
    set(&p, "x", 3, NULL); /* shadows origin's x */

    send(&origin, "describe");
    printf(" ");
    send(&p, "describe");
    printf("\n");
    set(&traits, "describe", 0, describe_angle); /* one change, every object sees it */
    send(&origin, "describe");
    printf(" ");
    send(&p, "describe");
    printf("\n");
    return 0;
}
```

```output for=slot-table
(0, 0) (3, 0)
<0 0> <3 0>
```

- Verdict: absent. C has structs with fixed members and no dispatch. A prototype object is a struct with a parent pointer and a table of named slots, and a lookup function walks the parent chain. A method slot holds a function pointer that takes the receiver as its argument. Changing a slot in the shared parent changes the behavior of every object that delegates to it.
