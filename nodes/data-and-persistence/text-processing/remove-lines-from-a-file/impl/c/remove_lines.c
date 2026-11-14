// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

// mkstemp, fchmod and fsync are in POSIX, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "remove_lines.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int remove_lines(const char *path, int start, int count) {
    if (start < 1 || count < 0) {
        errno = EINVAL;
        return -1;
    }
    FILE *src = fopen(path, "rb");
    if (src == NULL) {
        return -1;
    }
    struct stat st;
    char *tmp = NULL;
    FILE *dst = NULL;
    int fd = -1;
    if (fstat(fileno(src), &st) != 0) {
        goto fail;
    }
    // mkstemp changes the six X characters to make a name that no file has.
    // The name is path with a suffix, so it is in the same directory.
    size_t len = strlen(path);
    tmp = malloc(len + sizeof ".XXXXXX");
    if (tmp == NULL) {
        goto fail;
    }
    memcpy(tmp, path, len);
    memcpy(tmp + len, ".XXXXXX", sizeof ".XXXXXX");
    fd = mkstemp(tmp);
    if (fd < 0) {
        free(tmp);
        tmp = NULL;
        goto fail;
    }
    dst = fdopen(fd, "wb");
    if (dst == NULL) {
        close(fd);
        goto fail;
    }
    // n is the number of the line of the next byte. A long long does not
    // overflow, and n - start < count does not overflow either.
    long long n = 1;
    int removed = 0;
    int line_start = 1;
    int c;
    while ((c = getc(src)) != EOF) {
        if (n >= start && n - start < count) {
            removed += line_start;
        } else if (putc(c, dst) == EOF) {
            goto fail;
        }
        line_start = c == '\n';
        n += line_start;
    }
    if (ferror(src)) {
        goto fail;
    }
    if (removed == 0) {
        fclose(dst);
        unlink(tmp);
        free(tmp);
        fclose(src);
        return 0;
    }
    // mkstemp makes the file readable and writable only by its owner.
    if (fflush(dst) != 0 || fchmod(fd, st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)) != 0 ||
        fsync(fd) != 0) {
        goto fail;
    }
    int closed = fclose(dst);
    dst = NULL;
    if (closed != 0 || rename(tmp, path) != 0) {
        goto fail;
    }
    free(tmp);
    fclose(src);
    return removed;

fail:;
    int e = errno;
    if (dst != NULL) {
        fclose(dst);
    }
    if (tmp != NULL) {
        unlink(tmp);
        free(tmp);
    }
    fclose(src);
    errno = e;
    return -1;
}
