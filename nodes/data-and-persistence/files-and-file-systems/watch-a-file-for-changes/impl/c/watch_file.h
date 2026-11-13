// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

#ifndef WATCH_FILE_H
#define WATCH_FILE_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <time.h>

// The cases set the mtime to WATCH_BASE_MTIME + step, in seconds.
#define WATCH_BASE_MTIME 1700000000L

enum watch_change { WATCH_NONE, WATCH_CREATED, WATCH_DELETED, WATCH_MODIFIED };

// The last signature of a file: its size and its mtime.
struct file_watcher {
    const char *path;
    bool exists;
    off_t size;
    struct timespec mtime;
};

// Takes the first signature. Returns 0, or -1 with errno (EISDIR for a directory).
int watcher_init(struct file_watcher *w, const char *path);

// Sets *change to what changed since the last poll. Returns 0, or -1 with errno.
int watcher_poll(struct file_watcher *w, enum watch_change *change);

// The name of a change: none, created, deleted or modified.
const char *watch_change_name(enum watch_change change);

// The entry of the cases: applies each of the n steps, polls after each, and
// writes the n results in out. Returns 0, or -1 with errno.
int watch_changes(const char *path, const char *const *steps, size_t n, enum watch_change *out);

#endif
