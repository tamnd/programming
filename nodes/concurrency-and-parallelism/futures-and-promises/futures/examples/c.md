```c run id=join
#include <pthread.h>
#include <stdio.h>

struct job {
    int input;
    int result;
};

static void *square(void *arg) {
    struct job *job = arg;
    job->result = job->input * job->input;
    return NULL;
}

int main(void) {
    struct job job = {.input = 7};
    pthread_t thread;
    if (pthread_create(&thread, NULL, square, &job) != 0) {
        return 1;
    }
    pthread_join(thread, NULL);
    printf("%d\n", job.result);
    return 0;
}
```

```output for=join
49
```

- Verdict: absent. C has no future type. A thread writes its result into memory that the caller owns, and `pthread_join` or `thrd_join` waits for the thread. After the join, the caller can read the result safely.
- A real future in C needs a mutex and a condition variable, or a library such as libuv for callbacks on an event loop.
