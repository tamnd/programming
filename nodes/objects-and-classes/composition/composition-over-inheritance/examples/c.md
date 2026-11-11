```c run id=struct-members-and-function-pointers
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* A writer is a part: a function pointer and the data it needs. */
struct writer {
    void (*write)(void *ctx, const char *msg);
    void *ctx;
};

static void write_stdout(void *ctx, const char *msg)
{
    (void)ctx;
    puts(msg);
}

struct upper {
    struct writer next;
};

static void write_upper(void *ctx, const char *msg)
{
    struct upper *u = ctx;
    char buf[64];
    size_t i;
    for (i = 0; msg[i] != '\0' && i < sizeof buf - 1; i++)
        buf[i] = (char)toupper((unsigned char)msg[i]);
    buf[i] = '\0';
    u->next.write(u->next.ctx, buf);
}

/* A logger has-a writer and counts its lines. */
struct logger {
    const char *prefix;
    struct writer out;
    int lines;
};

static void logger_log(struct logger *l, const char *msg)
{
    char buf[64];
    snprintf(buf, sizeof buf, "%s%s", l->prefix, msg);
    l->out.write(l->out.ctx, buf);
    l->lines++;
}

/* A service contains a logger as its first member. */
struct service {
    struct logger log;
    int requests;
};

int main(void)
{
    struct writer out = {write_stdout, NULL};
    struct service svc = {{"svc: ", out, 0}, 0};
    logger_log(&svc.log, "ready");

    struct upper up = {out};
    svc.log.out = (struct writer){write_upper, &up}; /* swap the part */
    logger_log(&svc.log, "ready");
    printf("%d lines\n", svc.log.lines);
    return 0;
}
```

```output for=struct-members-and-function-pointers
svc: ready
SVC: READY
2 lines
```

- Verdict: idiom. C builds a struct from other structs as members, and a member that holds a function pointer and a context pointer acts as a replaceable part. `struct service` contains a `struct logger`, which contains a `struct writer`, and assigning a new writer changes where the logger writes. A wrapper writer such as `upper` holds the next writer and changes the message before it passes it on, which is the decorator pattern in C.
