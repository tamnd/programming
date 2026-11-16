```c run id=tree
#include <stdio.h>

struct node {
    const char *name;
    int size;
    struct node *first_child;
    struct node *next_sibling;
};

static void show(const struct node *n, int depth) {
    printf("%*s", depth * 2, "");
    if (n->first_child == NULL) {
        printf("%s %d\n", n->name, n->size);
        return;
    }
    printf("%s/\n", n->name);
    for (const struct node *c = n->first_child; c != NULL; c = c->next_sibling) {
        show(c, depth + 1);
    }
}

static int count_files(const struct node *n) {
    if (n->first_child == NULL) {
        return 1;
    }
    int count = 0;
    for (const struct node *c = n->first_child; c != NULL; c = c->next_sibling) {
        count += count_files(c);
    }
    return count;
}

static int total_size(const struct node *n) {
    int total = n->size;
    for (const struct node *c = n->first_child; c != NULL; c = c->next_sibling) {
        total += total_size(c);
    }
    return total;
}

static int height(const struct node *n) {
    int best = 0;
    for (const struct node *c = n->first_child; c != NULL; c = c->next_sibling) {
        int h = 1 + height(c);
        if (h > best) {
            best = h;
        }
    }
    return best;
}

int main(void) {
    struct node test_main = {"test_main.py", 150, NULL, NULL};
    struct node util = {"util.py", 80, NULL, NULL};
    struct node main_py = {"main.py", 300, NULL, &util};
    struct node tests = {"tests", 0, &test_main, NULL};
    struct node src = {"src", 0, &main_py, &tests};
    struct node readme = {"README.md", 120, NULL, &src};
    struct node project = {"project", 0, &readme, NULL};

    show(&project, 0);
    printf("files %d, total size %d, height %d\n", count_files(&project), total_size(&project),
           height(&project));
    return 0;
}
```

```output for=tree
project/
  README.md 120
  src/
    main.py 300
    util.py 80
  tests/
    test_main.py 150
files 4, total size 650, height 2
```

- Verdict: idiom. Each node has a pointer to its first child and a pointer to its next sibling, so every node has the same fixed size.
- The nodes live on the stack of `main`, so the program allocates no memory.
