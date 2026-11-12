// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-word-frequencies/.

#ifndef TOP_WORDS_H
#define TOP_WORDS_H

#include <stddef.h>

// A word and the number of times it occurs. word points into the buffer of
// the struct word_counts that holds it.
struct word_count {
    const char *word;
    size_t count;
};

// The result of top_words. Free it with word_counts_free.
struct word_counts {
    struct word_count *items;
    size_t len;
    char *buf;  // the lower case words, each ended by a null character
};

// top_words counts the words of the UTF-8 string text and stores the n most
// frequent ones in *out, by count from high to low, then by word in byte
// order, which for UTF-8 is code point order. It returns 0, or -1 if there is
// no memory.
int top_words(const char *text, size_t n, struct word_counts *out);

void word_counts_free(struct word_counts *wc);

#endif
