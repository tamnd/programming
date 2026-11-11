```c run id=convention
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Returns a new string. The caller owns it and must free it. */
static char *make_greeting(const char *name) {
    size_t size = strlen("hello, ") + strlen(name) + 1;
    char *text = malloc(size);
    if (text != NULL) {
        snprintf(text, size, "hello, %s", name);
    }
    return text;
}

/* Borrows the string. It does not keep or free it. */
static size_t count_vowels(const char *text) {
    size_t count = 0;
    for (; *text != '\0'; text++) {
        count += strchr("aeiou", *text) != NULL;
    }
    return count;
}

int main(void) {
    char *greeting = make_greeting("ada");
    if (greeting == NULL) {
        return 1;
    }
    printf("%s has %zu vowels\n", greeting, count_vowels(greeting));
    free(greeting);
    greeting = NULL;
    return 0;
}
```

```output for=convention
hello, ada has 4 vowels
```

- Verdict: absent. A C pointer does not say if it owns the memory. The API documentation states who frees each pointer, and `const` marks a pointer that only reads.
- The compiler does not check the rules. Static analyzers and sanitizers find some double frees, leaks and uses after free.
