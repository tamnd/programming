// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

#include "count_lines.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

int count_lines(const char *path, uintmax_t *newlines, uintmax_t *lines) {
    // "rb": on Windows, "r" would change CRLF to LF and stop at the byte 0x1A.
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return -1;
    }
    static char buf[64 * 1024];
    uintmax_t count = 0;
    char last = '\n';  // an empty file has no line without an LF
    size_t n;
    while ((n = fread(buf, 1, sizeof buf, f)) > 0) {
        for (const char *p = buf, *end = buf + n; (p = memchr(p, '\n', (size_t)(end - p))) != NULL; p++) {
            count++;
        }
        last = buf[n - 1];
    }
    // fread returns 0 both at the end of the file and on an error.
    if (ferror(f)) {
        int saved = errno;
        fclose(f);
        errno = saved != 0 ? saved : EIO;
        return -1;
    }
    fclose(f);
    *newlines = count;
    *lines = count + (last != '\n');
    return 0;
}
