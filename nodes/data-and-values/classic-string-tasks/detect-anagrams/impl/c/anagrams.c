// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

#include "anagrams.h"

#include <ctype.h>

// count adds step to the counter of each letter of text.
static void count(int counts[26], const char *text, int step) {
    for (const char *p = text; *p != '\0'; p++) {
        unsigned char c = (unsigned char)*p;
        if (isalpha(c)) {
            counts[tolower(c) - 'a'] += step;
        }
    }
}

bool are_anagrams(const char *a, const char *b) {
    int counts[26] = {0};
    count(counts, a, 1);
    count(counts, b, -1);
    for (int i = 0; i < 26; i++) {
        if (counts[i] != 0) {
            return false;
        }
    }
    return true;
}
