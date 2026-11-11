```c run id=pthread-actor
#include <pthread.h>
#include <stdio.h>

enum op { DEPOSIT, WITHDRAW, STOP };
struct msg { enum op op; int amount; };

#define CAP 256
static struct msg box[CAP];
static int head, tail;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t ready = PTHREAD_COND_INITIALIZER;

static void post(struct msg m)
{
    pthread_mutex_lock(&lock);
    box[tail++ % CAP] = m;
    pthread_cond_signal(&ready);
    pthread_mutex_unlock(&lock);
}

static struct msg take(void)
{
    pthread_mutex_lock(&lock);
    while (head == tail)
        pthread_cond_wait(&ready, &lock);
    struct msg m = box[head++ % CAP];
    pthread_mutex_unlock(&lock);
    return m;
}

static void *account(void *arg)
{
    int balance = 0; /* only this thread uses balance */
    for (;;) {
        struct msg m = take();
        if (m.op == DEPOSIT)
            balance += m.amount;
        else if (m.op == WITHDRAW && m.amount <= balance)
            balance -= m.amount;
        else if (m.op == STOP)
            break;
    }
    *(int *)arg = balance;
    return NULL;
}

static void *client(void *arg)
{
    (void)arg;
    for (int i = 0; i < 25; i++)
        post((struct msg){ DEPOSIT, 10 });
    return NULL;
}

int main(void)
{
    int result;
    pthread_t actor, clients[4];
    pthread_create(&actor, NULL, account, &result);
    for (int i = 0; i < 4; i++)
        pthread_create(&clients[i], NULL, client, NULL);
    for (int i = 0; i < 4; i++)
        pthread_join(clients[i], NULL);
    post((struct msg){ WITHDRAW, 1 });
    post((struct msg){ STOP, 0 });
    pthread_join(actor, NULL);
    printf("balance %d\n", result);
    return 0;
}
```

```output for=pthread-actor
balance 999
```

- Verdict: absent. C has threads in `<threads.h>` and POSIX threads, but no message queues between them. The mailbox here is a ring buffer with a mutex and a condition variable. The mutex is inside the mailbox only. The state of the actor needs no lock, because one thread owns it. The ring buffer does not check for overflow, so it is safe only while fewer than 256 messages wait at once, as in this example.
