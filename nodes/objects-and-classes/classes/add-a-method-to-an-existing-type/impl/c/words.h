// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

#ifndef WORDS_H
#define WORDS_H

// C has no methods, and its strings are arrays of char, so nothing can be
// added to the type. A function that takes the string as its first parameter
// does the job.

// word_count returns the number of runs of characters in the NUL-terminated
// string text that are not space, tab, newline or carriage return.
int word_count(const char *text);

#endif
