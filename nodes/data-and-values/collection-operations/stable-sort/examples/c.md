```c run id=qsort-with-position
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct staff {
    const char *name;
    const char *team;
    size_t pos; /* the original position, used to break ties */
};

/* qsort does not promise a stable sort, so the comparator uses pos when the teams are equal. */
static int by_team_then_pos(const void *a, const void *b) {
    const struct staff *x = a, *y = b;
    int c = strcmp(x->team, y->team);
    if (c != 0) return c;
    return (x->pos > y->pos) - (x->pos < y->pos);
}

int main(void) {
    struct staff s[] = {{"ann", "ops", 0}, {"bob", "dev", 0}, {"cy", "ops", 0}, {"di", "dev", 0}, {"ed", "ops", 0}};
    size_t n = sizeof s / sizeof s[0];
    for (size_t i = 0; i < n; i++) s[i].pos = i;

    qsort(s, n, sizeof s[0], by_team_then_pos);
    for (size_t i = 0; i < n; i++) printf("%s %s\n", s[i].team, s[i].name);
    return 0;
}
```

```output
dev bob
dev di
ops ann
ops cy
ops ed
```

- Verdict: idiom. `qsort` does not promise a stable sort. Each record stores its original position, and the comparator compares the position when the teams are equal, so the result is the same as a stable sort.
