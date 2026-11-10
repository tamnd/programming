```c fails expect=sanitizer:address timeout=30 id=missing-op
#include <stdio.h>

struct socket_ops {
    int (*open)(void);
    int (*send)(const char *data);
};

static int plain_open(void) {
    return 0;
}

// This family does not set send, so send is NULL.
static const struct socket_ops packet_ops = {.open = plain_open};

int main(void) {
    const struct socket_ops *ops = &packet_ops;
    printf("open: %d\n", ops->open());
    printf("send: %d\n", ops->send("hello")); // calls through NULL
}
```

A table of operations leaves one function pointer out, so the pointer is NULL. The call through it jumps to address zero. AddressSanitizer reports a `SEGV on unknown address`. This is the shape of CVE-2009-2692, where the kernel relied on every `proto_ops` structure being filled.

```c run flags="-fsanitize=address,undefined" id=stub
#include <stdio.h>

struct socket_ops {
    int (*open)(void);
    int (*send)(const char *data);
};

static int plain_open(void) {
    return 0;
}

// The stub refuses the operation with an error code.
static int no_send(const char *data) {
    (void)data;
    return -1;
}

// Every member has a value, so no call goes through NULL.
static const struct socket_ops packet_ops = {.open = plain_open, .send = no_send};

int main(void) {
    const struct socket_ops *ops = &packet_ops;
    printf("open: %d\n", ops->open());
    printf("send: %d\n", ops->send("hello"));
}
```

```output for=stub
open: 0
send: -1
```

- Verdict: absent. C has no type for "maybe a pointer". Every pointer can be NULL, and the compiler does not check that the code tests it.
- Check the result of every function that can return NULL, such as `malloc`, `fopen`, `getenv` and lookups.
- Fill every member of a table of function pointers with a function or a stub that returns an error.
- Build the tests with `-fsanitize=undefined`, which includes the check `-fsanitize=null`.
