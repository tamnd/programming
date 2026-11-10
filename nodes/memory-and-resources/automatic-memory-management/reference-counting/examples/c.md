```c run id=retain-release
#include <stdio.h>
#include <stdlib.h>

struct buffer {
    int refs;
    char name[16];
};

struct buffer *buffer_new(const char *name) {
    struct buffer *b = malloc(sizeof *b);
    if (b != NULL) {
        b->refs = 1;
        snprintf(b->name, sizeof b->name, "%s", name);
    }
    return b;
}

struct buffer *buffer_retain(struct buffer *b) {
    b->refs++;
    return b;
}

void buffer_release(struct buffer *b) {
    if (--b->refs == 0) {
        printf("free %s\n", b->name);
        free(b);
    }
}

int main(void) {
    struct buffer *b = buffer_new("frame");
    if (b == NULL) {
        return 1;
    }
    struct buffer *reader = buffer_retain(b);
    struct buffer *writer = buffer_retain(b);
    printf("refs %d\n", b->refs);
    buffer_release(b);
    buffer_release(writer);
    printf("refs %d\n", reader->refs);
    buffer_release(reader);
    printf("done\n");
}
```

```output
refs 3
refs 1
free frame
done
```

- Verdict: idiom. C has no reference counts. A library puts a count in the struct and gives a retain function and a release function, and every caller must pair them by hand.
- `buffer_new` returns the buffer with a count of 1, for the caller. The release that brings the count to zero frees the struct.
- A missing release is a leak, and an extra release frees the struct while another pointer still uses it.
- The plain `int` count is for one thread. A buffer that threads share needs an atomic count.
