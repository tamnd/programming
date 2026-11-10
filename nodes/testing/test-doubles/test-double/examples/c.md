```c run id=register
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* A seam in C: a struct of function pointers and a context pointer. */
typedef struct {
    bool (*exists)(void *ctx, const char *email);
    void (*add)(void *ctx, const char *email);
    void *ctx;
} Users;

typedef struct {
    void (*send)(void *ctx, const char *to, const char *body);
    void *ctx;
} Mailer;

static const char *register_user(const char *email, Users users, Mailer mailer) {
    if (users.exists(users.ctx, email)) {
        return "taken";
    }
    users.add(users.ctx, email);
    mailer.send(mailer.ctx, email, "Welcome!");
    return "created";
}

/* A fake store: a fixed array in memory. */
typedef struct {
    char emails[8][64];
    int count;
} FakeStore;

static bool fake_exists(void *ctx, const char *email) {
    FakeStore *store = ctx;
    for (int i = 0; i < store->count; i++) {
        if (strcmp(store->emails[i], email) == 0) {
            return true;
        }
    }
    return false;
}

static void fake_add(void *ctx, const char *email) {
    FakeStore *store = ctx;
    if (store->count == 8) {
        return; /* full: a fake must not write past its array */
    }
    snprintf(store->emails[store->count++], sizeof store->emails[0], "%s", email);
}

/* A spy: counts the calls and keeps the last recipient. */
typedef struct {
    int calls;
    char last_to[64];
} SpyLog;

static void spy_send(void *ctx, const char *to, const char *body) {
    SpyLog *log = ctx;
    log->calls++;
    snprintf(log->last_to, sizeof log->last_to, "%s", to);
    (void)body;
}

int main(void) {
    FakeStore store = {0};
    SpyLog log = {0};
    Users users = {fake_exists, fake_add, &store};
    Mailer mailer = {spy_send, &log};
    printf("%s\n", register_user("ada@example.com", users, mailer));
    printf("%s\n", register_user("ada@example.com", users, mailer));
    printf("send called %d time(s), last to %s\n", log.calls, log.last_to);
    return 0;
}
```

```output
created
taken
send called 1 time(s), last to ada@example.com
```

- Verdict: idiom. C has no interfaces. The seam is a struct of function pointers with a `void *` context, which is a vtable written by hand.
- Each double casts the context back to its own type. The compiler cannot check that the context and the functions match, so a mistake there is undefined behaviour.
- The fake store has a fixed size, and `fake_add` checks the bound before it writes. A fake in C has the same memory bugs as other C code, and an unchecked fake turns a test into an out-of-bounds write.
- The spy keeps only a count and the last recipient. A spy that records every call needs a buffer that grows, which C does not give.
