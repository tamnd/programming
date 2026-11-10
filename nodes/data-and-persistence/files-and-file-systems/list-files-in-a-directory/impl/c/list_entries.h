// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

#ifndef LIST_ENTRIES_H
#define LIST_ENTRIES_H

#include <stdbool.h>
#include <stddef.h>

// list_entries lists the entries under the directory root, and the whole tree
// if recursive is true. It stores in *out a new array of *n new strings: the
// relative paths, sorted by byte, with a "/" after each directory. A symbolic
// link is listed, but it is never followed. It returns 0. On an error it
// returns -1 and sets errno: ENOENT for a missing root, ENOTDIR for a root
// that is not a directory, or the error of another call.
int list_entries(const char *root, bool recursive, char ***out, size_t *n);

// free_entries frees the array of list_entries and its strings.
void free_entries(char **entries, size_t n);

#endif
