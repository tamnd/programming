// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

#include "linked_list.h"

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// C has no empty array, so an empty list is an array of one item with a count of 0.
#define CHECK(name, n, ops, m, want)                                             \
    do {                                                                         \
        const char *const o[] = ops;                                             \
        const int w[] = want;                                                    \
        int out[16] = {0};                                                       \
        ptrdiff_t got = run_ops(o, n, out);                                      \
        if (got != (m) || memcmp(out, w, (size_t)(m) * sizeof(int)) != 0) {      \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                          \
        }                                                                        \
    } while (0)

int main(void) {
    CHECK("deque", 5, L("push_back 1", "push_back 2", "push_front 0", "pop_back", "push_back 3"), 3, L(0, 1, 3));
    CHECK("remove-middle", 4, L("push_back 1", "push_back 2", "push_back 3", "remove 2"), 2, L(1, 3));
    CHECK("empty", 0, L(""), 0, L(0));
    CHECK("push-pop", 2, L("push_back 1", "pop_front"), 0, L(0));
    CHECK("single", 1, L("push_front 7"), 1, L(7));
    CHECK("remove-only", 3, L("push_back 5", "remove 5", "push_back 6"), 1, L(6));
    CHECK("remove-head", 4, L("push_back 1", "push_back 2", "remove 1", "push_front 0"), 2, L(0, 2));
    CHECK("remove-tail", 4, L("push_back 1", "push_back 2", "remove 2", "push_back 3"), 2, L(1, 3));
    CHECK("pop-both-ends", 5, L("push_back 1", "push_back 2", "pop_front", "pop_back", "push_front 3"), 1, L(3));
    CHECK("remove-absent", 2, L("push_back 1", "remove 9"), 1, L(1));
    CHECK("duplicates", 4, L("push_back 4", "push_back 5", "push_back 4", "remove 4"), 2, L(5, 4));
    CHECK("push-front-order", 3, L("push_front 1", "push_front 2", "push_front 3"), 3, L(3, 2, 1));
    CHECK("negative", 3, L("push_front -1", "push_back -2", "remove -1"), 1, L(-2));
    CHECK("limit", 4, L("push_back 2147483647", "push_front -2147483648", "remove 2147483647", "push_back 2147483647"), 2, L(INT_MIN, INT_MAX));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
