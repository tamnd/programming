// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

#ifndef LETTERS_H
#define LETTERS_H

// Writes the counts of the letters a to z in text to counts[0] to counts[25],
// in the order of the alphabet. Upper case letters count as lower case
// letters.
void count_letters(const char *text, long long counts[26]);

#endif
