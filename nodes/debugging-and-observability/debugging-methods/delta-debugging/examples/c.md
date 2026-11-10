```c run id=ddmin
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static int tests = 0;

// Stands for one run of the browser: it crashes on a <SELECT ...> tag.
static bool browser_crashes(const char *text) {
    tests++;
    const char *start = strstr(text, "<SELECT");
    return start != NULL && strchr(start, '>') != NULL;
}

// Reduces chars in place to a 1-minimal part on which fails is still true.
static void ddmin(char *chars, bool (*fails)(const char *)) {
    char candidate[256];
    size_t n = 2;
    while (strlen(chars) >= 2) {
        size_t size = strlen(chars);
        bool reduced = false;
        for (size_t i = 0; i < n && !reduced; i++) { // the subsets
            size_t lo = i * size / n, hi = (i + 1) * size / n;
            snprintf(candidate, sizeof candidate, "%.*s", (int)(hi - lo), chars + lo);
            if (fails(candidate)) { // reduce to subset
                strcpy(chars, candidate);
                n = 2;
                printf("subset     %zu\n", strlen(chars));
                reduced = true;
            }
        }
        for (size_t i = 0; i < n && !reduced; i++) { // the complements
            size_t lo = i * size / n, hi = (i + 1) * size / n;
            snprintf(candidate, sizeof candidate, "%.*s%s", (int)lo, chars, chars + hi);
            if (fails(candidate)) { // reduce to complement
                strcpy(chars, candidate);
                n = n - 1 > 2 ? n - 1 : 2;
                printf("complement %zu\n", strlen(chars));
                reduced = true;
            }
        }
        if (reduced) {
            continue;
        }
        if (n == size) { // each single character was removed
            break;
        }
        n = 2 * n < size ? 2 * n : size; // increase granularity
    }
}

int main(void) {
    const char *page = "<td align=left><SELECT NAME=\"priority\" MULTIPLE SIZE=7>";
    char minimal[256];
    snprintf(minimal, sizeof minimal, "%s", page);
    ddmin(minimal, browser_crashes);
    printf("%zu characters reduced to %s in %d tests\n", strlen(page), minimal, tests);
}
```

```output for=ddmin
complement 42
complement 28
complement 21
complement 17
complement 14
complement 13
complement 12
complement 10
complement 9
complement 8
55 characters reduced to <SELECT> in 147 tests
```

- Verdict: idiom. The C library has no delta debugging.
- `snprintf` with `%.*s` builds each part in a fixed buffer, so the program does not allocate. The buffer limits the input to 255 bytes.
- `ddmin` changes the string in place, and a global variable counts the tests, because a C function pointer has no captured state.
