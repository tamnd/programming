```c fails expect=sanitizer:leak timeout=30 id=lost-pointer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns a copy of the longest line. The caller frees it.
char *longest(const char *lines[], size_t count) {
    char *best = NULL;
    for (size_t i = 0; i < count; i++) {
        if (best == NULL || strlen(lines[i]) > strlen(best)) {
            best = malloc(strlen(lines[i]) + 1); // the old block loses its last pointer
            if (best == NULL) {
                return NULL;
            }
            strcpy(best, lines[i]);
        }
    }
    return best;
}

int main(void) {
    const char *lines[] = {"red", "green", "yellow"};
    char *best = longest(lines, 3);
    if (best == NULL) {
        return 1;
    }
    printf("longest: %s\n", best);
    free(best);
}
```

LeakSanitizer reports two direct leaks at exit, the copies of `red` and `green`. Each `malloc` overwrites `best`, so no pointer to the old block remains and no code can free it.

```c run id=grow-in-place
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns a copy of the longest line. The caller frees it.
char *longest(const char *lines[], size_t count) {
    char *best = NULL;
    for (size_t i = 0; i < count; i++) {
        size_t len = strlen(lines[i]);
        if (best == NULL || len > strlen(best)) {
            // A failed realloc keeps the old block, so keep its pointer too.
            char *bigger = realloc(best, len + 1);
            if (bigger == NULL) {
                free(best);
                return NULL;
            }
            best = bigger;
            memcpy(best, lines[i], len + 1);
        }
    }
    return best;
}

int main(void) {
    const char *lines[] = {"red", "green", "yellow"};
    char *best = longest(lines, 3);
    if (best == NULL) {
        return 1;
    }
    printf("longest: %s\n", best);
    free(best);
}
```

```output
longest: yellow
```

- Verdict: absent. C has no owner for a heap block. A block leaks when the program overwrites or drops the last pointer to it before `free`.
- The fix keeps one block and grows it with `realloc`. The result goes into a temporary, because `best = realloc(best, n)` loses the old block when `realloc` fails.
- Build tests with `-fsanitize=address`. LeakSanitizer then lists each leaked block with the stack of its `malloc` when the process exits.
