```c fails expect=sanitizer:address timeout=30 id=heap-copy
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns a copy of name on the heap.
char *copy_name(const char *name) {
    char *copy = malloc(strlen(name)); // no room for the final '\0'
    strcpy(copy, name);
    return copy;
}

int main(void) {
    char *name = copy_name("alice");
    printf("%s\n", name);
    free(name);
}
```

AddressSanitizer reports a `heap-buffer-overflow` and a `WRITE of size 6` in `strcpy`. The buffer has five bytes, and `strcpy` writes the five letters and the final `'\0'`. Without the sanitizer, the program can print `alice` and exit with status 0, which hides the bug.

```c fails expect=sanitizer:address timeout=30 id=stack-cat
#include <stdio.h>
#include <string.h>

void greet(const char *name) {
    char line[16];
    strcpy(line, "hello, ");
    strcat(line, name); // no check that name fits
    puts(line);
}

int main(void) {
    greet("bob");
    greet("AAAAAAAAAAAAAAAAAAAAAAAA");
}
```

The second call uses the classic demonstration input, a long run of `A`. AddressSanitizer reports a `stack-buffer-overflow` in `strcat`. On the stack, the bytes after `line` can hold the return address of `greet`, which is why this form is dangerous.

```c run flags="-fsanitize=address,undefined" id=bounded
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns a copy of name on the heap, or NULL.
char *copy_name(const char *name) {
    size_t size = strlen(name) + 1; // room for the final '\0'
    char *copy = malloc(size);
    if (copy != NULL) {
        memcpy(copy, name, size);
    }
    return copy;
}

// Writes a greeting into line and reports if it did not fit.
void greet(const char *name) {
    char line[16];
    int n = snprintf(line, sizeof line, "hello, %s", name);
    if (n < 0 || (size_t)n >= sizeof line) {
        printf("refused: the name needs %d bytes, the buffer has %zu\n", n + 1, sizeof line);
        return;
    }
    puts(line);
}

int main(void) {
    char *name = copy_name("alice");
    if (name == NULL) {
        return 1;
    }
    printf("%s\n", name);
    free(name);
    greet("bob");
    greet("AAAAAAAAAAAAAAAAAAAAAAAA");
}
```

```output for=bounded
alice
hello, bob
refused: the name needs 32 bytes, the buffer has 16
```

- Verdict: absent. C has no bounds check on arrays, pointers or the string functions. A write past the end is undefined behavior, and the program does not have to fail.
- The fix computes each size from the data, adds one byte for `'\0'`, and uses functions that take the size of the destination. `snprintf` never writes more than its size argument, and its return value tells the program that the text did not fit.
- `strncpy` is not a full fix. CWE-787 warns that it may not end the string with `'\0'` when the source is as long as the destination.
- Build tests with `-fsanitize=address`. Build releases with the hardening of the compiler, such as `FORTIFY_SOURCE` and stack canaries. They detect some overflows at run time, and the program then exits.
