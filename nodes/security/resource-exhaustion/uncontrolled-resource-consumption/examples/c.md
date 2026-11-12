```c fails expect=sanitizer:address timeout=30 id=deep-recursion
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Parse nested brackets with one call for each level. Returns the number of lists. */
static long parse(const char *s, size_t *i) {
    long count = 1;
    (*i)++; /* skip '[' */
    while (s[*i] == '[') {
        count += parse(s, i);
    }
    (*i)++; /* skip ']' */
    return count;
}

int main(void) {
    /* One million levels of nesting is 2 MB of input. */
    size_t depth = 1000000;
    char *input = malloc(2 * depth + 1);
    if (input == NULL) return 1;
    memset(input, '[', depth);
    memset(input + depth, ']', depth);
    input[2 * depth] = '\0';

    size_t i = 0;
    long lists = parse(input, &i);
    printf("parsed %ld lists\n", lists);
    free(input);
    return 0;
}
```

The parser calls itself once for each `[`. One million levels use more stack than the thread has, and AddressSanitizer reports a `stack-overflow`. A build without the sanitizer stops with a signal from the operating system. Either way, one request stops the whole process.

```c run flags="-fsanitize=address,undefined" id=depth-limit
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEPTH 64

/* Returns the number of lists, or -1 if the input is nested deeper than MAX_DEPTH. */
static long parse(const char *s, size_t *i, int depth) {
    if (depth > MAX_DEPTH) return -1;
    long count = 1;
    (*i)++; /* skip '[' */
    while (s[*i] == '[') {
        long child = parse(s, i, depth + 1);
        if (child < 0) return -1;
        count += child;
    }
    if (s[*i] != ']') return -1;
    (*i)++; /* skip ']' */
    return count;
}

static void handle(const char *input) {
    size_t i = 0;
    long lists = parse(input, &i, 1);
    if (lists < 0) {
        printf("400 rejected: nesting deeper than %d levels or malformed\n", MAX_DEPTH);
    } else {
        printf("200 parsed %ld lists\n", lists);
    }
}

int main(void) {
    size_t depth = 1000000;
    char *input = malloc(2 * depth + 1);
    if (input == NULL) return 1;
    memset(input, '[', depth);
    memset(input + depth, ']', depth);
    input[2 * depth] = '\0';

    handle(input);
    handle("[[][[]]]");
    free(input);
    return 0;
}
```

```output for=depth-limit
400 rejected: nesting deeper than 64 levels or malformed
200 parsed 4 lists
```

The depth is an argument, and the parser stops at 64 levels. The check of `]` also rejects input that ends early.

- Verdict: idiom. C has no limit on recursion and no check on the stack. A depth counter or an explicit stack on the heap with a maximum size is the usual fix.
- Check every size from the input before `malloc`, and check the result of `malloc`. A huge size from a length field can make the allocation fail, or wrap around in the arithmetic before it, as described in [[integer-overflow]].
- `setrlimit` limits the memory, the CPU time and the open files of a process on POSIX systems, as a second layer.
