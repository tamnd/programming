// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

#include "linked_list.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int value;
    struct node *prev;
    struct node *next;
};

struct list {
    struct node *head;
    struct node *tail;
};

static int push_front(struct list *l, int value) {
    struct node *n = malloc(sizeof *n);
    if (n == NULL) {
        return -1;
    }
    n->value = value;
    n->prev = NULL;
    n->next = l->head;
    if (l->head == NULL) {
        l->tail = n;
    } else {
        l->head->prev = n;
    }
    l->head = n;
    return 0;
}

static int push_back(struct list *l, int value) {
    struct node *n = malloc(sizeof *n);
    if (n == NULL) {
        return -1;
    }
    n->value = value;
    n->next = NULL;
    n->prev = l->tail;
    if (l->tail == NULL) {
        l->head = n;
    } else {
        l->tail->next = n;
    }
    l->tail = n;
    return 0;
}

// unlink_node takes n out of the list and frees it.
static void unlink_node(struct list *l, struct node *n) {
    if (n->prev == NULL) {
        l->head = n->next;
    } else {
        n->prev->next = n->next;
    }
    if (n->next == NULL) {
        l->tail = n->prev;
    } else {
        n->next->prev = n->prev;
    }
    free(n);
}

static void free_list(struct list *l) {
    struct node *n = l->head;
    while (n != NULL) {
        struct node *next = n->next;  // read next before free
        free(n);
        n = next;
    }
    l->head = NULL;
    l->tail = NULL;
}

static int run_one(struct list *l, const char *op) {
    char name[16];
    int value = 0;
    int got = sscanf(op, "%15s %d", name, &value);
    if (got == 2 && strcmp(name, "push_front") == 0) {
        return push_front(l, value);
    }
    if (got == 2 && strcmp(name, "push_back") == 0) {
        return push_back(l, value);
    }
    if (got == 1 && strcmp(name, "pop_front") == 0 && l->head != NULL) {
        unlink_node(l, l->head);
        return 0;
    }
    if (got == 1 && strcmp(name, "pop_back") == 0 && l->tail != NULL) {
        unlink_node(l, l->tail);
        return 0;
    }
    if (got == 2 && strcmp(name, "remove") == 0) {
        for (struct node *n = l->head; n != NULL; n = n->next) {
            if (n->value == value) {
                unlink_node(l, n);
                break;
            }
        }
        return 0;
    }
    return -1;
}

ptrdiff_t run_ops(const char *const *ops, size_t n, int *out) {
    struct list l = {NULL, NULL};
    for (size_t i = 0; i < n; i++) {
        if (run_one(&l, ops[i]) != 0) {
            free_list(&l);
            return -1;
        }
    }
    ptrdiff_t count = 0;
    for (struct node *p = l.head; p != NULL; p = p->next) {
        out[count++] = p->value;
    }
    free_list(&l);
    return count;
}
