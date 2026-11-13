```c run id=loops-for-each-step
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *title;
    int words;
    const char *tags[2];
} Article;

static int has_tag(const Article *a, const char *tag) {
    for (int i = 0; i < 2 && a->tags[i] != NULL; i++) {
        if (strcmp(a->tags[i], tag) == 0) {
            return 1;
        }
    }
    return 0;
}

static int by_words_desc(const void *x, const void *y) {
    const Article *a = x;
    const Article *b = y;
    return (b->words > a->words) - (b->words < a->words);
}

static int calls = 0;

static int score(const Article *a) {
    calls++;
    return a->words;
}

int main(void) {
    const Article articles[] = {
        {"Intro to NoSQL", 1200, {"nosql", "databases"}},
        {"Event sourcing", 800, {"architecture", NULL}},
        {"Graph stores", 1500, {"nosql", "graphs"}},
        {"Refactoring loops", 600, {"refactoring", NULL}},
        {"Key-value stores", 900, {"nosql", NULL}},
    };
    const int n = (int)(sizeof articles / sizeof articles[0]);

    // C has no pipeline operations. Each step is a loop over an array, or qsort.
    Article nosql[5];
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (has_tag(&articles[i], "nosql")) {
            nosql[count++] = articles[i];
        }
    }
    qsort(nosql, (size_t)count, sizeof nosql[0], by_words_desc);
    printf("%s, %s\n", nosql[0].title, nosql[1].title);

    int total = 0;
    for (int i = 0; i < count; i++) {
        total += nosql[i].words;
    }
    printf("%d\n", total);

    // Eager: one loop computes every score into an array, a second loop filters it.
    int all[5];
    for (int i = 0; i < n; i++) {
        all[i] = score(&articles[i]);
    }
    int first = -1;
    for (int i = 0; i < n && first < 0; i++) {
        if (all[i] > 1000) {
            first = all[i];
        }
    }
    printf("eager %d %d\n", first, calls);

    // Fused: one loop does map, filter and take, and stops at the first match.
    calls = 0;
    first = -1;
    for (int i = 0; i < n; i++) {
        int s = score(&articles[i]);
        if (s > 1000) {
            first = s;
            break;
        }
    }
    printf("lazy %d %d\n", first, calls);
    return 0;
}
```

```output
Graph stores, Intro to NoSQL
3600
eager 1200 5
lazy 1200 1
```

- Verdict: idiom. C has no pipeline operations and no closures. Each step is a loop over an array, and qsort does the sort with a comparison function. The eager form computes all 5 scores into an array and then filters it. The fused form does map, filter and take in one loop and stops with break, so score runs once. This fused loop is the work that a lazy pipeline does for the program.
