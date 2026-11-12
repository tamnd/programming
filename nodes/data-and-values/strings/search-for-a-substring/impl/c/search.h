// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-for-a-substring/.

#ifndef SEARCH_H
#define SEARCH_H

// find returns the code point index of the first match of the UTF-8 string sub
// in the UTF-8 string s, or -1 if there is no match.
long find(const char *s, const char *sub);

// find_last returns the code point index of the last match of sub in s, or -1.
long find_last(const char *s, const char *sub);

#endif
