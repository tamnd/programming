// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

#ifndef FOLLOW_FILE_H
#define FOLLOW_FILE_H

#include <stddef.h>
#include <sys/types.h>

// Reads the lines that are added to a file, one poll at a time, as tail -F
// does. The follower keeps the file open, so it can read the rest of the file
// after a rename or a delete.
struct follower {
    char *path;
    int fd;  // -1 if no file is open
    off_t offset;
    char *pending;  // the bytes after the last LF
    size_t npending, cap;
};

// Gets one line, with no LF and no NUL at the end. Returns 0, or -1 with
// errno set to stop the poll.
typedef int (*follow_emit)(const char *line, size_t len, void *arg);

// Opens the file at path and starts at its end. Returns 0, or -1 with errno
// set. errno is ENOENT if no file is at path.
int follower_open(struct follower *f, const char *path);

// Calls emit with each complete line that was added since the last poll.
// Returns 0, or -1 with errno set.
int follower_poll(struct follower *f, follow_emit emit, void *arg);

// Calls follower_poll each interval_ms milliseconds, until stop returns
// nonzero. Returns 0, or -1 with errno set.
int follower_follow(struct follower *f, unsigned interval_ms, int (*stop)(void *), follow_emit emit, void *arg);

// Closes the file and frees the memory of the follower.
void follower_close(struct follower *f);

// Writes initial to path, makes a follower and does the steps, which end at a
// NULL. Each poll gives one string, with an LF after each line. Puts the
// strings in a new array in *polls, and their number in *npolls. The caller
// frees each string and the array. Returns 0, or -1 with errno set.
int follow_file(const char *path, const char *initial, const char *const steps[], char ***polls, size_t *npolls);

#endif
