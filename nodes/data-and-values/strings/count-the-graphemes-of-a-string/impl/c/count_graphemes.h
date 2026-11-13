// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.
#ifndef COUNT_GRAPHEMES_H
#define COUNT_GRAPHEMES_H

#include <stddef.h>

// count_graphemes returns the number of extended grapheme clusters in the
// UTF-8 string s. strlen(s) counts bytes.
size_t count_graphemes(const char *s);

#endif
