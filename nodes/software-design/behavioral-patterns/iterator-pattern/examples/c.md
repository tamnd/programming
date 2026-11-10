```c run id=tree
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct node {
    int key;
    struct node *left, *right;
};

static struct node *insert(struct node *root, int key) {
    struct node **link = &root;
    while (*link != NULL) {
        link = key < (*link)->key ? &(*link)->left : &(*link)->right;
    }
    *link = calloc(1, sizeof **link);
    if (*link == NULL) {
        exit(1);
    }
    (*link)->key = key;
    return root;
}

static void destroy(struct node *n) {
    if (n != NULL) {
        destroy(n->left);
        destroy(n->right);
        free(n);
    }
}

/* The iterator: the stack of nodes that are left. 64 levels is enough for this example. */
struct tree_iter {
    const struct node *stack[64];
    int top;
};

static void push_left(struct tree_iter *it, const struct node *n) {
    for (; n != NULL; n = n->left) {
        it->stack[it->top++] = n;
    }
}

static void tree_iter_init(struct tree_iter *it, const struct node *root) {
    it->top = 0;
    push_left(it, root);
}

/* Writes the next key to *key and returns true, or returns false at the end. */
static bool tree_iter_next(struct tree_iter *it, int *key) {
    if (it->top == 0) {
        return false;
    }
    const struct node *n = it->stack[--it->top];
    push_left(it, n->right);
    *key = n->key;
    return true;
}

static bool same_keys(const struct node *a, const struct node *b) {
    struct tree_iter left, right;
    tree_iter_init(&left, a);
    tree_iter_init(&right, b);
    int x, y;
    for (;;) {
        bool has_x = tree_iter_next(&left, &x);
        bool has_y = tree_iter_next(&right, &y);
        if (!has_x || !has_y) {
            return has_x == has_y;
        }
        if (x != y) {
            return false;
        }
    }
}

static struct node *from_keys(const int *keys, int count) {
    struct node *root = NULL;
    for (int i = 0; i < count; i++) {
        root = insert(root, keys[i]);
    }
    return root;
}

int main(void) {
    const int ka[] = {5, 3, 8, 1, 4, 9}, kb[] = {1, 3, 4, 5, 8, 9}, kc[] = {1, 3, 4, 5, 8};
    struct node *a = from_keys(ka, 6), *b = from_keys(kb, 6), *c = from_keys(kc, 5);

    struct tree_iter it;
    int key, sum = 0;
    printf("in order:");
    for (tree_iter_init(&it, a); tree_iter_next(&it, &key);) {
        printf(" %d", key);
        sum += key;
    }
    printf("\nsum: %d\nfirst two:", sum);
    tree_iter_init(&it, a);
    for (int i = 0; i < 2 && tree_iter_next(&it, &key); i++) {
        printf(" %d", key);
    }
    printf("\nsame keys as b: %s\n", same_keys(a, b) ? "yes" : "no");
    printf("same keys as c: %s\n", same_keys(a, c) ? "yes" : "no");
    destroy(a);
    destroy(b);
    destroy(c);
    return 0;
}
```

```output
in order: 1 3 4 5 8 9
sum: 30
first two: 1 3
same keys as b: yes
same keys as c: no
```

- Verdict: idiom. C has no iterator protocol and no loop statement that calls one. A struct that holds the position and a `next` function that returns `false` at the end is the usual form.
- The caller owns the iterator struct, here on the stack. Two iterators over the same tree are two structs, so `same_keys` runs two traversals at once.
- Nothing ties the iterator to the tree. If the program frees or changes the tree while an iterator holds node pointers, the next call reads freed memory, and the compiler does not warn.
- The fixed array of 64 entries limits the depth of the tree. A real program grows the stack with `realloc`, or stores parent pointers in the nodes and needs no stack.
