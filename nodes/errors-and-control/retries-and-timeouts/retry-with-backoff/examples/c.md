```c run id=errno
#include <errno.h>
#include <stdio.h>

static int calls = 0;
static long now_ms = 0;

static int send_report(int failures, int error) {
    calls++;
    if (calls <= failures) {
        errno = error;
        return -1;
    }
    return 0;
}

static unsigned next_random(unsigned *state) {
    *state = *state * 1103515245u + 12345u;
    return (*state >> 16) & 0x7fff;
}

static int with_retry(int failures, int error, unsigned *rng) {
    long base = 100, cap = 1000;
    for (int attempt = 0; attempt < 5; attempt++) {
        if (send_report(failures, error) == 0)
            return 0;
        if (errno != EAGAIN && errno != EINTR)
            return -1;
        long limit = base << attempt;
        if (limit > cap)
            limit = cap;
        long half = limit / 2;
        long delay = half + (long)(next_random(rng) % (unsigned)(half + 1));
        printf("  t=%4ld ms: attempt %d failed (%s); wait %ld ms\n", now_ms, attempt + 1,
               errno == EAGAIN ? "EAGAIN" : "EINTR", delay);
        now_ms += delay;
    }
    return -1;
}

int main(void) {
    unsigned rng = 1;
    puts("busy");
    calls = 0, now_ms = 0;
    int ok = with_retry(2, EAGAIN, &rng);
    printf("  result %d after %d calls\n", ok, calls);
    puts("bad argument");
    calls = 0, now_ms = 0;
    int rc = with_retry(1, EINVAL, &rng);
    printf("  result %d after %d calls, errno is %s\n", rc, calls, errno == EINVAL ? "EINVAL" : "other");
    return 0;
}
```

```output
busy
  t=   0 ms: attempt 1 failed (EAGAIN); wait 58 ms
  t=  58 ms: attempt 2 failed (EAGAIN); wait 101 ms
  result 0 after 3 calls
bad argument
  result -1 after 1 calls, errno is EINVAL
```

- Verdict: absent. C has no retry facility and no exceptions. A failed call returns `-1` and sets `errno`, and the caller reads `errno` to decide.
- `EAGAIN` and `EINTR` mean that the same call can succeed later, so the loop retries them. `EINVAL` means that the arguments are wrong, so a retry cannot help.
- This loop uses "equal jitter": half of the backoff is fixed and the other half is random, so no wait is very short.

Retries at several layers multiply. Here three layers each make up to four attempts, so one request from the user reaches the failing database 64 times.

```c wrong id=layers
#include <stdio.h>

static int database_calls = 0;

static int database(void) {
    database_calls++;
    return -1;
}

static int with_retries(int (*call)(void)) {
    for (int attempt = 0; attempt < 4; attempt++)
        if (call() == 0)
            return 0;
    return -1;
}

static int backend(void) { return with_retries(database); }
static int frontend(void) { return with_retries(backend); }
static int client(void) { return with_retries(frontend); }

int main(void) {
    client();
    printf("database calls for one user request: %d\n", database_calls);
    return 0;
}
```

```output
database calls for one user request: 64
```
