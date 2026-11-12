// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

#ifndef CASE_H
#define CASE_H

// to_upper returns a new string with a to z of the UTF-8 string s changed to
// A to Z, or NULL if there is no memory. The caller frees the result.
char *to_upper(const char *s);

// to_lower returns a new string with A to Z changed to a to z, or NULL if
// there is no memory. The caller frees the result.
char *to_lower(const char *s);

#endif
