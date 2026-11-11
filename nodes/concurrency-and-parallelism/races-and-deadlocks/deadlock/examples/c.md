```c run id=edeadlk
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_t m;
    pthread_mutex_init(&m, &attr);

    pthread_mutex_lock(&m);
    int rc = pthread_mutex_lock(&m);
    printf("second lock: %s\n", rc == EDEADLK ? "EDEADLK" : strerror(rc));
    rc = pthread_mutex_trylock(&m);
    printf("trylock: %s\n", rc == EBUSY ? "EBUSY" : strerror(rc));
    pthread_mutex_unlock(&m);

    rc = pthread_join(pthread_self(), NULL);
    printf("self-join: %s\n", rc == EDEADLK ? "EDEADLK" : strerror(rc));

    pthread_mutex_destroy(&m);
    pthread_mutexattr_destroy(&attr);
    return 0;
}
```

```output for=edeadlk
second lock: EDEADLK
trylock: EBUSY
self-join: EDEADLK
```

- Verdict: stdlib. The example uses POSIX threads. A `PTHREAD_MUTEX_NORMAL` mutex deadlocks when its owner locks it again. A `PTHREAD_MUTEX_ERRORCHECK` mutex returns `EDEADLK` instead, and a `PTHREAD_MUTEX_RECURSIVE` mutex counts the locks.
- `pthread_mutex_trylock` returns `EBUSY` when the mutex is locked, also for its owner, unless the mutex is recursive. It is the try-lock for the back off method.
- POSIX allows `pthread_join` to fail with `EDEADLK` when it detects a deadlock. The C library of the example returns it for a self-join, as Linux does.
