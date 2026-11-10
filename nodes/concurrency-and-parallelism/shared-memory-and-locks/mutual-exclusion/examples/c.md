```c run id=counter
#include <errno.h>
#include <pthread.h>
#include <stdio.h>

static pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;
static long counter = 0;

static void *add_many(void *arg) {
    long times = *(long *)arg;
    for (long i = 0; i < times; i++) {
        pthread_mutex_lock(&counter_lock);
        counter++;
        pthread_mutex_unlock(&counter_lock);
    }
    return NULL;
}

int main(void) {
    pthread_t workers[4];
    long times = 100000;
    for (int i = 0; i < 4; i++) pthread_create(&workers[i], NULL, add_many, &times);
    for (int i = 0; i < 4; i++) pthread_join(workers[i], NULL);
    printf("total: %ld\n", counter);

    pthread_mutexattr_t attributes;
    pthread_mutexattr_init(&attributes);
    pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_t checked;
    pthread_mutex_init(&checked, &attributes);
    pthread_mutex_lock(&checked);
    int again = pthread_mutex_lock(&checked);
    printf("second lock: %s\n", again == EDEADLK ? "EDEADLK" : "no error");
    pthread_mutex_unlock(&checked);
    pthread_mutex_destroy(&checked);
    pthread_mutexattr_destroy(&attributes);
    return 0;
}
```

```output
total: 400000
second lock: EDEADLK
```

- Verdict: stdlib. This example uses POSIX threads, `pthread_mutex_t`. The C standard has `mtx_t` in `<threads.h>`, but the header is optional, and the C library of macOS does not have it.
- C has no guards. Each path out of the critical section needs its own `pthread_mutex_unlock`. A `goto` to one cleanup label keeps the paths together.
- The mutex type decides what a second lock by the owner does. `PTHREAD_MUTEX_ERRORCHECK` returns `EDEADLK`, `PTHREAD_MUTEX_RECURSIVE` counts the levels, and for the default type the result is undefined.
- `pthread_rwlock_t` is the POSIX reader-writer lock, with `pthread_rwlock_rdlock` for readers and `pthread_rwlock_wrlock` for the writer. Check the return value of each call in real code.
