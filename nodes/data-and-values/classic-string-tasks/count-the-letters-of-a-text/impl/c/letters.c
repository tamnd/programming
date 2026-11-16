// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

#include "letters.h"

#include <ctype.h>

void count_letters(const char *text, long long counts[26]) {
    for (int i = 0; i < 26; i++) {
        counts[i] = 0;
    }
    for (const char *p = text; *p != '\0'; p++) {
        unsigned char c = (unsigned char)*p;
        if (isalpha(c)) {
            counts[tolower(c) - 'a']++;
        }
    }
}
