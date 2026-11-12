// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

#ifndef TRIM_H
#define TRIM_H

// trim returns a new string with the ASCII white space at the start and at the
// end of s removed, or NULL if there is no memory. The caller frees the result.
char *trim(const char *s);

// trim_start returns a new string with the white space at the start removed,
// or NULL if there is no memory. The caller frees the result.
char *trim_start(const char *s);

#endif
