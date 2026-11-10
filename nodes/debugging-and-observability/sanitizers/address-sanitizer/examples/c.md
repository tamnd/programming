```c fails expect=sanitizer:address timeout=30 id=heap-overflow
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Copies a name into a new buffer. The size forgets the terminating '\0'.
char *copy_name(const char *name) {
    char *copy = malloc(strlen(name));
    if (copy != NULL) {
        strcpy(copy, name); // writes one byte past the end
    }
    return copy;
}

int main(void) {
    char *name = copy_name("alice");
    if (name == NULL) {
        return 1;
    }
    printf("%s\n", name);
    free(name);
}
```

The buffer has 5 bytes, and `strcpy` writes 6. AddressSanitizer reports a `heap-buffer-overflow` in `strcpy`, called from `copy_name`. Without the sanitizer, the program usually prints `alice`, because the extra byte lands in padding that the allocator does not use.

```c run flags="-fsanitize=address" id=shadow
#include <sanitizer/asan_interface.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char *buf = malloc(13);
    if (buf == NULL) {
        return 1;
    }
    // One digit for each byte from buf - 2 to buf + 17: 1 means poisoned.
    for (int i = -2; i < 18; i++) {
        printf("%d", __asan_address_is_poisoned(buf + i));
    }
    printf("\n");
    free(buf);
}
```

```output for=shadow
11000000000000011111
```

The run asks the ASan runtime which bytes it treats as unaddressable. The two bytes before the block are the left redzone. The 13 bytes of the block are addressable. The bytes from `buf + 13` on are poisoned: three bytes at the end of the second 8-byte granule, then the right redzone.

```c fails expect=sanitizer:address timeout=30 id=stack-overflow
#include <stdio.h>

int sum(const int *values, int count) {
    int total = 0;
    for (int i = 0; i <= count; i++) { // <= reads one element too many
        total += values[i];
    }
    return total;
}

int main(void) {
    int scores[4] = {3, 1, 4, 1};
    printf("%d\n", sum(scores, 4));
}
```

The compiler puts redzones around `scores` in the stack frame of `main`. The read of `values[4]` hits the right redzone, and ASan reports a `stack-buffer-overflow`.

```c run flags="-fsanitize=address,undefined" id=fixed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Copies a name into a new buffer, with room for the terminating '\0'.
char *copy_name(const char *name) {
    size_t size = strlen(name) + 1;
    char *copy = malloc(size);
    if (copy != NULL) {
        memcpy(copy, name, size);
    }
    return copy;
}

int sum(const int *values, int count) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += values[i];
    }
    return total;
}

int main(void) {
    char *name = copy_name("alice");
    if (name == NULL) {
        return 1;
    }
    int scores[4] = {3, 1, 4, 1};
    printf("%s %d\n", name, sum(scores, 4));
    free(name);
}
```

```output for=fixed
alice 9
```

```c fails expect=sanitizer:undefined timeout=30 id=signed-overflow
#include <limits.h>
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int main(void) {
    printf("%d\n", add(INT_MAX, 1));
}
```

UndefinedBehaviorSanitizer reports `signed integer overflow: 2147483647 + 1 cannot be represented in type 'int'`. ASan does not check arithmetic, so this needs `-fsanitize=undefined`.

```c fails expect=sanitizer:thread timeout=30 id=data-race
#include <pthread.h>
#include <stdio.h>

static long counter = 0;

static void *work(void *arg) {
    (void)arg;
    for (int i = 0; i < 1000; i++) {
        counter++; // two threads write without a lock
    }
    return NULL;
}

int main(void) {
    pthread_t a, b;
    pthread_create(&a, NULL, work, NULL);
    pthread_create(&b, NULL, work, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    printf("%ld\n", counter);
}
```

ThreadSanitizer reports a `data race` on the global `counter`, with the stacks of the two writes. ASan and TSan cannot be in the same build, so a project builds and tests twice.

- Verdict: library. ASan is a part of Clang and GCC, not of the C language. `-fsanitize=address` adds the checks at compile time and links the runtime library.
- C has no bounds check of its own, so ASan finds errors that the language leaves as undefined behavior. See [[out-of-bounds-write]] and [[out-of-bounds-read]].
- `<sanitizer/asan_interface.h>` lets a program poison and unpoison its own memory. A custom allocator uses `ASAN_POISON_MEMORY_REGION` to mark free blocks, and the macro does nothing in a build without ASan.
- Leak detection (LSan) is not supported by the Apple toolchain that recorded these examples, so this file has no leak example. See [[memory-leaks]].
