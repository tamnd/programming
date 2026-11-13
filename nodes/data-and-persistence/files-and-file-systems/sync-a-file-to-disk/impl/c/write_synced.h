// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

#ifndef WRITE_SYNCED_H
#define WRITE_SYNCED_H

#include <stddef.h>

// Syncs the entries of the directory dir. It returns 0, or -1 with errno set.
int sync_directory(const char *dir);

// Writes the len bytes of text to path, syncs the file, closes it, and syncs
// the directory of path. It returns 0, or -1 with errno set.
int write_synced(const char *path, const char *text, size_t len);

#endif
