// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

#ifndef MAKE_SYMLINK_H
#define MAKE_SYMLINK_H

#include <stddef.h>
#include <sys/types.h>

// make_symlink makes a symbolic link at link that holds target, reads the
// target back with readlink into buf, which has size bytes, and ends it with a
// NUL byte. target is stored as it is: it need not exist, and a relative
// target is read from the directory of link. It returns the length of the
// target, or -1 with errno: EEXIST when link exists, also as a dangling link,
// ENOENT when the directory of link does not exist, and ERANGE when buf is too
// small. After ERANGE, the link exists.
ssize_t make_symlink(const char *target, const char *link, char *buf, size_t size);

#endif
