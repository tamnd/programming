// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

#ifndef WORD_FREQUENCIES_H
#define WORD_FREQUENCIES_H

#include <stddef.h>

struct word_freq {
    char *word;    // a run of ASCII letters in lower case, ended by a NUL
    size_t count;  // the number of times the word occurs
};

// word_frequencies reads the file at path and finds its n most frequent words.
// It stores a new array of at most n entries in *out and its length in *len,
// sorted by count from high to low, then by word, and returns 0. Free the
// array with word_frequencies_free. On an error it returns -1 and sets errno:
// the error of fopen or getc, or ENOMEM.
int word_frequencies(const char *path, size_t n, struct word_freq **out, size_t *len);

// word_frequencies_free frees the array and the words that word_frequencies gave.
void word_frequencies_free(struct word_freq *words, size_t len);

#endif
