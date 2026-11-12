// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-word-frequencies/.

#include "top_words.h"

#include <stdlib.h>
#include <string.h>

// A byte is part of a word if it is an ASCII letter or digit, or if it is not
// ASCII. Each byte of a multibyte UTF-8 sequence is 0x80 or more. The code
// does not call isalnum, because its result depends on the locale.
static int is_word_byte(unsigned char c) {
    return c >= 0x80 || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

// strcmp compares the bytes as unsigned char, so this is code point order.
static int by_word(const void *a, const void *b) {
    return strcmp(*(char *const *)a, *(char *const *)b);
}

static int by_count_then_word(const void *a, const void *b) {
    const struct word_count *x = a, *y = b;
    if (x->count != y->count) {
        return x->count < y->count ? 1 : -1;
    }
    return strcmp(x->word, y->word);
}

int top_words(const char *text, size_t n, struct word_counts *out) {
    *out = (struct word_counts){0};
    size_t len = strlen(text);
    // The copy has a null character in place of each separator byte, so each
    // word in it is a C string.
    char *buf = malloc(len + 1);
    // There are at most len / 2 + 1 words.
    char **words = malloc((len / 2 + 1) * sizeof *words);
    if (buf == NULL || words == NULL) {
        free(buf);
        free(words);
        return -1;
    }
    size_t nwords = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)text[i];
        if (!is_word_byte(c)) {
            buf[i] = '\0';
            continue;
        }
        buf[i] = (c >= 'A' && c <= 'Z') ? (char)(c - 'A' + 'a') : (char)c;
        if (i == 0 || buf[i - 1] == '\0') {
            words[nwords++] = &buf[i];
        }
    }
    buf[len] = '\0';

    // The C library has no map. Sort the words, so equal words are next to
    // each other, and count each run.
    qsort(words, nwords, sizeof *words, by_word);
    struct word_count *items = malloc((nwords + 1) * sizeof *items);
    if (items == NULL) {
        free(buf);
        free(words);
        return -1;
    }
    size_t nitems = 0;
    for (size_t i = 0; i < nwords; i++) {
        if (nitems > 0 && strcmp(items[nitems - 1].word, words[i]) == 0) {
            items[nitems - 1].count++;
        } else {
            items[nitems++] = (struct word_count){words[i], 1};
        }
    }
    free(words);

    // qsort is not stable, but no two items are equal under this order.
    qsort(items, nitems, sizeof *items, by_count_then_word);
    *out = (struct word_counts){items, nitems < n ? nitems : n, buf};
    return 0;
}

void word_counts_free(struct word_counts *wc) {
    free(wc->items);
    free(wc->buf);
    *wc = (struct word_counts){0};
}
