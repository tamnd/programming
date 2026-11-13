// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

#include "group_by_length.h"

#include <string.h>

size_t group_by_length(const char *const *words, size_t n, const char **sorted, struct word_group *out) {
    // An insertion sort is stable: it moves a word only past longer words.
    // qsort does not keep the order of equal items.
    for (size_t i = 0; i < n; i++) {
        size_t j = i;
        size_t len = strlen(words[i]);
        while (j > 0 && strlen(sorted[j - 1]) > len) {
            sorted[j] = sorted[j - 1];
            j--;
        }
        sorted[j] = words[i];
    }
    size_t k = 0;
    for (size_t i = 0; i < n; i++) {
        size_t len = strlen(sorted[i]);
        if (k > 0 && out[k - 1].len == len) {
            out[k - 1].count++;
        } else {
            out[k].len = len;
            out[k].start = i;
            out[k].count = 1;
            k++;
        }
    }
    return k;
}
