```c run id=sums
#include <pthread.h>
#include <stdio.h>

struct job {
    long start, stop, sum;
};

static void *partial_sum(void *arg) {
    struct job *job = arg;
    for (long n = job->start; n < job->stop; n++) {
        job->sum += n;
    }
    return &job->sum;
}

int main(void) {
    pthread_t threads[4];
    struct job jobs[4];
    for (int i = 0; i < 4; i++) {
        jobs[i] = (struct job){i * 250L, (i + 1) * 250L, 0};
        if (pthread_create(&threads[i], NULL, partial_sum, &jobs[i]) != 0) {
            return 1;
        }
    }
    long total = 0;
    for (int i = 0; i < 4; i++) {
        void *result;
        pthread_join(threads[i], &result);
        printf("thread %d: %ld\n", i, *(long *)result);
        total += *(long *)result;
    }
    printf("total: %ld\n", total);
    return 0;
}
```

```output for=sums
thread 0: 31125
thread 1: 93625
thread 2: 156125
thread 3: 218625
total: 499500
```

- Verdict: stdlib. This example uses POSIX threads. ISO C defines `<threads.h>`, but it is optional: an implementation that defines `__STDC_NO_THREADS__` does not provide it, and the macOS C library is one such implementation.
- `pthread_create` takes a function of type `void *(void *)` and one argument. A struct carries the input and the result.
- `pthread_join` waits for the thread and stores its return value through the second argument. The pointer must point to memory that is still alive, here an element of `jobs` in `main`.
- Join or detach every thread. Otherwise the system cannot reclaim its storage.
