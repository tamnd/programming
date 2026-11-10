```c run id=atomic
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

static atomic_int count = 0;

static void *work(void *arg) {
    (void)arg;
    for (int i = 0; i < 100000; i++) {
        atomic_fetch_add(&count, 1);
    }
    return NULL;
}

int main(void) {
    pthread_t threads[2];
    for (int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, work, NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("%d\n", atomic_load(&count));
}
```

```output for=atomic
200000
```

```c fails expect=sanitizer:thread flags="-fsanitize=thread" id=race
#include <pthread.h>
#include <stdio.h>

static int count = 0;

static void *work(void *arg) {
    (void)arg;
    for (int i = 0; i < 100000; i++) {
        count++;
    }
    return NULL;
}

int main(void) {
    pthread_t threads[2];
    for (int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, work, NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }
}
```

- Verdict: native. `_Atomic` is a type qualifier of the language, and `atomic_int` from `<stdatomic.h>` is `_Atomic int`. `atomic_fetch_add` is one atomic read-modify-write.
- The second program has a plain `static int count`. Any data race in C is undefined behaviour, so it has no output block. ThreadSanitizer, enabled with `-fsanitize=thread`, reports `WARNING: ThreadSanitizer: data race`.
- C11 makes threads and atomics optional, and an implementation without them defines `__STDC_NO_THREADS__` or `__STDC_NO_ATOMICS__`. The C library of the test host has no `<threads.h>`, so these programs use POSIX threads.
