// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

#include "version.h"

#include <ctype.h>
#include <stdlib.h>

static bool read_number(const char **p, long *out) {
    if (!isdigit((unsigned char)**p)) {
        return false;
    }
    char *end;
    *out = strtol(*p, &end, 10);
    *p = end;
    return true;
}

bool version_parse(const char *text, Version *out) {
    const char *p = text;
    if (!read_number(&p, &out->major) || *p++ != '.') return false;
    if (!read_number(&p, &out->minor) || *p++ != '.') return false;
    if (!read_number(&p, &out->patch)) return false;
    return *p == '\0';
}

// (x > y) - (x < y) is -1, 0 or 1, and it cannot overflow as x - y can.
static int compare_long(long x, long y) {
    return (x > y) - (x < y);
}

int version_compare(const Version *a, const Version *b) {
    int c = compare_long(a->major, b->major);
    if (c != 0) return c;
    c = compare_long(a->minor, b->minor);
    return c != 0 ? c : compare_long(a->patch, b->patch);
}

struct item {
    Version v;
    const char *text;
};

static int compare_items(const void *x, const void *y) {
    const struct item *a = x, *b = y;
    return version_compare(&a->v, &b->v);
}

int sort_versions(const char **versions, size_t n) {
    if (n == 0) {
        return 0;
    }
    struct item *items = malloc(n * sizeof *items);
    if (items == NULL) {
        return -1;
    }
    for (size_t i = 0; i < n; i++) {
        if (!version_parse(versions[i], &items[i].v)) {
            free(items);
            return -1;
        }
        items[i].text = versions[i];
    }
    qsort(items, n, sizeof *items, compare_items);
    for (size_t i = 0; i < n; i++) {
        versions[i] = items[i].text;
    }
    free(items);
    return 0;
}
