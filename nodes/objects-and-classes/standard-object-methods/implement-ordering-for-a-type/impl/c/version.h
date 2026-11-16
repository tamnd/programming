// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

#ifndef VERSION_H
#define VERSION_H

#include <stdbool.h>
#include <stddef.h>

// A version MAJOR.MINOR.PATCH. C has no methods, so the natural order is a
// comparison function that qsort and bsearch can take.
typedef struct {
    long major;
    long minor;
    long patch;
} Version;

// version_parse reads a version such as "1.10.0" into *out and returns true,
// or returns false if the text is not three numbers joined by dots.
bool version_parse(const char *text, Version *out);

// version_compare returns a negative number, zero or a positive number when
// *a is less than, equal to or greater than *b. It compares the numbers, most
// important first.
int version_compare(const Version *a, const Version *b);

// sort_versions sorts the n strings in place by the natural order of the
// versions that they hold. It returns 0, or -1 if a string is not a version
// or if memory runs out; the array is then unchanged.
int sort_versions(const char **versions, size_t n);

#endif
