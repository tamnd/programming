// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

#include "word_frequencies.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A hash table with open addressing and linear probing. A slot with a NULL
// word is empty. The capacity is a power of 2, and the table grows before it
// is half full, so a probe always finds an empty slot.
struct table {
    struct word_freq *slots;
    size_t cap;
    size_t used;
};

// FNV-1a, 64 bits.
static uint64_t hash(const char *s, size_t len) {
    uint64_t h = 14695981039346656037u;
    for (size_t i = 0; i < len; i++) {
        h ^= (unsigned char)s[i];
        h *= 1099511628211u;
    }
    return h;
}

static struct word_freq *find(struct word_freq *slots, size_t cap, const char *word, size_t len) {
    size_t i = hash(word, len) & (cap - 1);
    while (slots[i].word != NULL && strcmp(slots[i].word, word) != 0) {
        i = (i + 1) & (cap - 1);
    }
    return &slots[i];
}

static int grow(struct table *t) {
    size_t cap = t->cap * 2;
    struct word_freq *slots = calloc(cap, sizeof *slots);
    if (slots == NULL) {
        return -1;
    }
    for (size_t i = 0; i < t->cap; i++) {
        if (t->slots[i].word != NULL) {
            *find(slots, cap, t->slots[i].word, strlen(t->slots[i].word)) = t->slots[i];
        }
    }
    free(t->slots);
    t->slots = slots;
    t->cap = cap;
    return 0;
}

// add adds 1 to the count of the word of len bytes, ended by a NUL.
static int add(struct table *t, const char *word, size_t len) {
    if (2 * (t->used + 1) > t->cap && grow(t) != 0) {
        return -1;
    }
    struct word_freq *slot = find(t->slots, t->cap, word, len);
    if (slot->word == NULL) {
        slot->word = malloc(len + 1);
        if (slot->word == NULL) {
            return -1;
        }
        memcpy(slot->word, word, len + 1);
        t->used++;
    }
    slot->count++;
    return 0;
}

static int compare(const void *a, const void *b) {
    const struct word_freq *x = a, *y = b;
    if (x->count != y->count) {
        return x->count > y->count ? -1 : 1;
    }
    // strcmp compares the bytes as unsigned char.
    return strcmp(x->word, y->word);
}

void word_frequencies_free(struct word_freq *words, size_t len) {
    for (size_t i = 0; i < len; i++) {
        free(words[i].word);
    }
    free(words);
}

int word_frequencies(const char *path, size_t n, struct word_freq **out, size_t *len) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return -1;
    }
    struct table t = {calloc(16, sizeof(struct word_freq)), 16, 0};
    size_t wcap = 64, wlen = 0;
    char *word = malloc(wcap);
    int err = 0;
    if (t.slots == NULL || word == NULL) {
        err = ENOMEM;
    }
    // getc reads from the buffer of the stream. The current word stays in
    // word across the reads of the buffer.
    for (int c = 0; err == 0 && c != EOF;) {
        c = getc(f);
        if (c >= 'A' && c <= 'Z') {
            c += 'a' - 'A';
        }
        if (c >= 'a' && c <= 'z') {
            if (wlen + 1 == wcap) {
                char *bigger = realloc(word, wcap * 2);
                if (bigger == NULL) {
                    err = ENOMEM;
                    break;
                }
                word = bigger;
                wcap *= 2;
            }
            word[wlen++] = (char)c;
        } else if (wlen > 0) {
            // A byte that is not a letter, or the end of the file, ends the word.
            word[wlen] = '\0';
            if (add(&t, word, wlen) != 0) {
                err = ENOMEM;
            }
            wlen = 0;
        }
    }
    if (err == 0 && ferror(f)) {
        err = errno != 0 ? errno : EIO;
    }
    fclose(f);
    free(word);
    // Move the words to the start of the array, sort them, and keep n.
    size_t used = 0;
    for (size_t i = 0; t.slots != NULL && i < t.cap; i++) {
        if (t.slots[i].word != NULL) {
            t.slots[used++] = t.slots[i];
        }
    }
    if (err != 0) {
        word_frequencies_free(t.slots, used);
        errno = err;
        return -1;
    }
    qsort(t.slots, used, sizeof *t.slots, compare);
    size_t keep = used < n ? used : n;
    for (size_t i = keep; i < used; i++) {
        free(t.slots[i].word);
    }
    *out = t.slots;
    *len = keep;
    return 0;
}
