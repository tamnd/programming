```c run id=sensor
#include <stdio.h>

typedef void (*callback)(void *context, double value);

struct observer {
    callback fn;
    void *context;
};

enum { MAX_OBSERVERS = 8 };

struct sensor {
    struct observer observers[MAX_OBSERVERS];
    int count;
};

static int subscribe(struct sensor *s, callback fn, void *context) {
    if (s->count == MAX_OBSERVERS) {
        return -1;
    }
    s->observers[s->count++] = (struct observer){fn, context};
    return 0;
}

static void unsubscribe(struct sensor *s, callback fn, void *context) {
    for (int i = 0; i < s->count; i++) {
        if (s->observers[i].fn == fn && s->observers[i].context == context) {
            s->observers[i] = s->observers[--s->count];
            return;
        }
    }
}

static void set(struct sensor *s, double value) {
    for (int i = 0; i < s->count; i++) {
        s->observers[i].fn(s->observers[i].context, value);
    }
}

static void display(void *context, double value) {
    (void)context;
    printf("display: %.1f\n", value);
}

static void alarm(void *context, double value) {
    const double *limit = context;
    if (value > *limit) {
        printf("alarm: %.1f is above %.0f\n", value, *limit);
    }
}

int main(void) {
    struct sensor sensor = {0};
    double limit = 30;
    subscribe(&sensor, display, NULL);
    subscribe(&sensor, alarm, &limit);
    set(&sensor, 21.5);
    set(&sensor, 30.5);
    unsubscribe(&sensor, display, NULL);
    set(&sensor, 31.0);
    return 0;
}
```

```output for=sensor
display: 21.5
display: 30.5
alarm: 30.5 is above 30
alarm: 31.0 is above 30
```

- Verdict: idiom. C has no closures, so an observer is a function pointer and a `void *` context that holds its state. The alarm gets its limit through the context.
- The subject stores the observers in a fixed array, so it needs no allocation. `subscribe` reports a full array with `-1`.
- `unsubscribe` moves the last observer into the free slot. This is cheap, but it changes the order of notification. An observer that unsubscribes itself during `set` would also make the loop skip the observer that moved.
- The subject does not own the context. The caller must keep it valid until it unsubscribes.
