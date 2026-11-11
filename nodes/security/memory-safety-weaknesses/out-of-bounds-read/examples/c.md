```c fails expect=sanitizer:address timeout=30 id=claimed-length
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns a copy of the first claimed bytes of payload.
unsigned char *echo(const unsigned char *payload, size_t received, size_t claimed) {
    (void)received; // the real size is ignored
    unsigned char *reply = malloc(claimed);
    if (reply != NULL) {
        memcpy(reply, payload, claimed); // claimed comes from the sender
    }
    return reply;
}

int main(void) {
    size_t received = 5;
    unsigned char *payload = malloc(received);
    if (payload == NULL) {
        return 1;
    }
    memcpy(payload, "hello", received);
    unsigned char *reply = echo(payload, received, 64);
    if (reply != NULL) {
        printf("reply of 64 bytes starts with %.5s\n", (const char *)reply);
    }
    free(reply);
    free(payload);
}
```

AddressSanitizer reports a `heap-buffer-overflow` for a `READ of size 64`. The payload has 5 bytes, and the copy reads 59 more.

```c fails expect=sanitizer:address timeout=30 id=no-terminator
#include <stdio.h>
#include <string.h>

int main(void) {
    char name[5];
    memcpy(name, "alice", 5); // five letters, no room for '\0'
    printf("length: %zu\n", strlen(name));
}
```

`strlen` reads until it finds a zero byte. The array holds five letters and no `'\0'`, so the read goes past the end of the array on the stack. AddressSanitizer reports a `stack-buffer-overflow`.

```c run flags="-fsanitize=address,undefined" id=bounded
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>

int main(void) {
    char name[5];
    memcpy(name, "alice", 5);
    size_t length = strnlen(name, sizeof name);
    printf("length: %zu\n", length);
    printf("name: %.*s\n", (int)length, name);
}
```

```output for=bounded
length: 5
name: alice
```

- Verdict: absent. C does not check reads. A pointer carries no size, and an index is not compared with the length of the array.
- Pass the size of each buffer with the buffer, and compare every length from input with it before a copy.
- Use `strnlen`, `memchr` and `%.*s` on data that may lack a final `'\0'`.
- Check a signed index at both ends. CWE-125 shows a check of `index < len` alone, which lets a negative index read before the buffer.
