```c run id=queue
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#define CAP 2

typedef struct {
    pthread_mutex_t m;
    pthread_cond_t not_full, not_empty;
    int buf[CAP];
    int head, count;
    bool closed;
} channel;

static void ch_send(channel *c, int v) {
    pthread_mutex_lock(&c->m);
    while (c->count == CAP) pthread_cond_wait(&c->not_full, &c->m);
    c->buf[(c->head + c->count) % CAP] = v;
    c->count++;
    pthread_cond_signal(&c->not_empty);
    pthread_mutex_unlock(&c->m);
}

static bool ch_recv(channel *c, int *out) { /* false: closed and drained */
    pthread_mutex_lock(&c->m);
    while (c->count == 0 && !c->closed) pthread_cond_wait(&c->not_empty, &c->m);
    bool ok = c->count > 0;
    if (ok) {
        *out = c->buf[c->head];
        c->head = (c->head + 1) % CAP;
        c->count--;
        pthread_cond_signal(&c->not_full);
    }
    pthread_mutex_unlock(&c->m);
    return ok;
}

static void ch_close(channel *c) {
    pthread_mutex_lock(&c->m);
    c->closed = true;
    pthread_cond_broadcast(&c->not_empty);
    pthread_mutex_unlock(&c->m);
}

static channel jobs = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER,
                       PTHREAD_COND_INITIALIZER};

static void *produce(void *arg) {
    int id = *(int *)arg;
    for (int i = 1; i <= 3; i++) ch_send(&jobs, id * 10 * i);
    return NULL;
}

static void *consume(void *arg) {
    int v, *total = arg;
    while (ch_recv(&jobs, &v)) *total += v;
    return NULL;
}

int main(void) {
    pthread_t p[2], c;
    int ids[2] = {1, 2}, total = 0;
    pthread_create(&c, NULL, consume, &total);
    for (int i = 0; i < 2; i++) pthread_create(&p[i], NULL, produce, &ids[i]);
    for (int i = 0; i < 2; i++) pthread_join(p[i], NULL);
    ch_close(&jobs);
    pthread_join(c, NULL);
    printf("total: %d\n", total);
    return 0;
}
```

```output for=queue
total: 180
```

- Verdict: idiom. ISO C and POSIX have no channel. A ring buffer guarded by a `pthread_mutex_t`, with two condition variables, is the usual form.
- Each `pthread_cond_wait` sits in a `while` loop that tests the condition again, because a wait can return spuriously.
- Pipes (`pipe`) carry bytes between processes or threads. They are a channel of bytes, not of typed values.
