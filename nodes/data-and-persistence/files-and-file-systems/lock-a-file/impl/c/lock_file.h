// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

#ifndef LOCK_FILE_H
#define LOCK_FILE_H

enum lock_mode { LOCK_MODE_NONE, LOCK_MODE_SHARED, LOCK_MODE_EXCLUSIVE };

// Opens path, which must exist, and asks for a shared or an exclusive flock
// lock on it, without a wait. It returns the descriptor that holds the lock.
// It returns -1 with errno EWOULDBLOCK when another open file has a lock that
// conflicts, and -1 with another errno on an error.
int try_lock_file(const char *path, enum lock_mode mode);

// Releases the lock of fd and closes fd. It returns 0, or -1 with errno set.
int unlock_file(int fd);

// The entry of the cases: takes held, then asks for want. It returns 1 when
// the second lock was given, 0 when it was not, and -1 with errno on an error.
int try_lock(const char *path, enum lock_mode held, enum lock_mode want);

#endif
