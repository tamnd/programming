// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

#ifndef LEVENSHTEIN_H
#define LEVENSHTEIN_H

// Returns the minimum number of one-character insertions, deletions and
// substitutions that change a into b, or -1 if there is no memory for the
// two rows of the table.
int levenshtein(const char *a, const char *b);

#endif
