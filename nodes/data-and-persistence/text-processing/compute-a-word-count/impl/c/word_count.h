// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

#ifndef WORD_COUNT_H
#define WORD_COUNT_H

#include <stdint.h>

struct word_count {
    uintmax_t lines;  // the number of LF bytes
    uintmax_t words;  // runs of one or more code points that are not White_Space
    uintmax_t bytes;
};

// word_count reads the UTF-8 file at path and stores the counts in *out. It
// returns 0. On an error it returns -1 and sets errno: EILSEQ for bytes that
// are not valid UTF-8, or the error of fopen or getline.
int word_count(const char *path, struct word_count *out);

#endif
