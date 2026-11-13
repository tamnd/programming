// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

#include "rename_file.h"

#include <stdio.h>

// ISO C rename does not say what happens when dst exists. POSIX rename
// replaces dst atomically, and this code needs POSIX.
int rename_file(const char *src, const char *dst) {
    return rename(src, dst) == 0 ? 0 : -1;
}
