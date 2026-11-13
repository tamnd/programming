// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/normalize-unicode-text/.
#ifndef NORMALIZE_H
#define NORMALIZE_H

// normalize returns a new UTF-8 string with s in the form "NFC" or "NFD", for
// the code points of the contract. The caller frees it. It returns NULL if
// there is no memory.
char *normalize(const char *s, const char *form);

#endif
