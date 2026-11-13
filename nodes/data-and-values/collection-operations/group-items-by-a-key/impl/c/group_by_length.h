// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

#ifndef GROUP_BY_LENGTH_H
#define GROUP_BY_LENGTH_H

#include <stddef.h>

// A group is sorted[start .. start + count), the words with the length len.
struct word_group {
    size_t len;
    size_t start;
    size_t count;
};

// group_by_length writes the n words to sorted in ascending order of the
// length, and keeps the input order of words with the same length. It writes
// one group for each length to out, and returns the number of groups. sorted
// and out must each have room for n items.
size_t group_by_length(const char *const *words, size_t n, const char **sorted, struct word_group *out);

#endif
