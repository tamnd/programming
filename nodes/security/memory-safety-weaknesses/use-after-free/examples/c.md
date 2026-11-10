```c fails expect=sanitizer:address timeout=30 id=free-then-use
#include <stdio.h>
#include <stdlib.h>

struct session {
    char user[16];
    int logged_in;
};

struct session *open_session(const char *user) {
    struct session *s = calloc(1, sizeof *s);
    if (s != NULL) {
        snprintf(s->user, sizeof s->user, "%s", user);
        s->logged_in = 1;
    }
    return s;
}

void close_session(struct session *s) {
    free(s);
}

int main(void) {
    struct session *s = open_session("alice");
    if (s == NULL) {
        return 1;
    }
    close_session(s);
    printf("%s logged in: %d\n", s->user, s->logged_in); // s points to freed memory
}
```

AddressSanitizer reports a `heap-use-after-free` on the read in `main`. Without the sanitizer, the program can print the old values, because `free` does not clear the memory. It can also print values of a new object that reuses the block.

```c run flags="-fsanitize=address,undefined" id=null-after-free
#include <stdio.h>
#include <stdlib.h>

struct session {
    char user[16];
    int logged_in;
};

struct session *open_session(const char *user) {
    struct session *s = calloc(1, sizeof *s);
    if (s != NULL) {
        snprintf(s->user, sizeof s->user, "%s", user);
        s->logged_in = 1;
    }
    return s;
}

// Frees the session and clears the pointer of the caller.
void close_session(struct session **s) {
    free(*s);
    *s = NULL;
}

int main(void) {
    struct session *s = open_session("alice");
    if (s == NULL) {
        return 1;
    }
    printf("%s logged in: %d\n", s->user, s->logged_in);
    close_session(&s);
    if (s == NULL) {
        printf("the session is closed\n");
    }
}
```

```output for=null-after-free
alice logged in: 1
the session is closed
```

- Verdict: absent. C has no ownership or lifetime rules in the language. After `free`, each copy of the pointer is dangling, and nothing marks it.
- `close_session` takes the address of the pointer and sets it to `NULL`. CWE-416 lists this as defence in depth: a later use is then a null pointer dereference, which crashes instead of reading reused memory. It does not clear other copies of the pointer.
- The real fix is a rule for each object: one owner frees it, and no other code keeps a pointer past that point. Write the rule down in the comment of each function that takes or returns a pointer.
- Build tests with `-fsanitize=address`. It also reports a double free, a peer weakness, CWE-415.
