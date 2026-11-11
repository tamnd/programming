```c run id=stdatomic
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>

static atomic_int hits;

static void *work(void *arg) {
    (void)arg;
    for (int i = 0; i < 100000; i++) {
        atomic_fetch_add_explicit(&hits, 1, memory_order_relaxed);
    }
    return NULL;
}

int main(void) {
    pthread_t threads[4];
    for (int t = 0; t < 4; t++) {
        pthread_create(&threads[t], NULL, work, NULL);
    }
    for (int t = 0; t < 4; t++) {
        pthread_join(threads[t], NULL);
    }
    printf("%d\n", atomic_load(&hits));

    int expected = 400000;
    bool swapped = atomic_compare_exchange_strong(&hits, &expected, 0);
    printf("%d %d\n", swapped, atomic_load(&hits));
    return 0;
}
```

```output for=stdatomic
400000
1 0
```

- Verdict: stdlib. C11 added the `_Atomic` qualifier, `<stdatomic.h>` and the same memory orders as C++. Operations without `_explicit` use sequentially consistent order.
- Before C11, programs used compiler builtins, such as the GCC `__atomic` functions, or platform APIs. `volatile` does not make an access atomic and does not order other accesses.
