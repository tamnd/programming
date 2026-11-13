// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

#include "sort_by_length.h"

#include <stdlib.h>
#include <string.h>

// qsort gives the comparison function pointers to two items of the array.
// Each item is a pointer to a word.
static int compare(const void *pa, const void *pb) {
    const char *a = *(const char *const *)pa;
    const char *b = *(const char *const *)pb;
    size_t la = strlen(a);
    size_t lb = strlen(b);
    if (la != lb) {
        // A subtraction of two size_t values cannot be negative.
        return la < lb ? -1 : 1;
    }
    return strcmp(a, b);
}

void sort_by_length(const char **words, size_t n) {
    if (n > 1) {
        qsort(words, n, sizeof words[0], compare);
    }
}
