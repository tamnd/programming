// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

#include "words.h"

#include <stdbool.h>

static bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int word_count(const char *text) {
    int count = 0;
    bool in_word = false;
    for (const char *p = text; *p != '\0'; p++) {
        bool space = is_space(*p);
        if (!space && !in_word) {
            count++;
        }
        in_word = !space;
    }
    return count;
}
