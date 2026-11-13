```c run id=loops-into-arrays
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *words[] = {"apple", "fig", "banana", "kiwi", "cherry"};
    size_t n = sizeof words / sizeof words[0];

    // C has no comprehension. A loop fills an array and counts the items.
    size_t lengths[5];
    size_t count = 0;
    for (size_t i = 0; i < n; i++) {
        size_t len = strlen(words[i]);
        if (len > 3) {
            lengths[count++] = len;
        }
    }
    for (size_t i = 0; i < count; i++) {
        printf("%zu%s", lengths[i], i + 1 < count ? " " : "\n");
    }

    // A set of first letters: one flag for each letter.
    bool seen[26] = {false};
    for (size_t i = 0; i < n; i++) {
        seen[words[i][0] - 'a'] = true;
    }
    for (int c = 0; c < 26; c++) {
        if (seen[c]) {
            putchar('a' + c);
        }
    }
    putchar('\n');

    // Two loops nest like two for clauses.
    const char *ys = "ab";
    for (int x = 1; x <= 2; x++) {
        for (const char *y = ys; *y != '\0'; y++) {
            bool last = x == 2 && y[1] == '\0';
            printf("%d%c%s", x, *y, last ? "\n" : " ");
        }
    }
    return 0;
}
```

```output
5 6 4 6
abcfk
1a 1b 2a 2b
```

- Verdict: idiom. C has no comprehension syntax and no growable list in the standard library. The program fills an array that is large enough and counts the items, keeps a set of letters as an array of flags, and nests two loops for the pairs.
