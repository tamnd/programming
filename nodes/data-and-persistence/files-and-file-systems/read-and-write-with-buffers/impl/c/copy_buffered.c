// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

#include "copy_buffered.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

long long copy_buffered(const char *src, const char *dst, long size) {
    if (size < 1) {
        errno = EINVAL;
        return -1;
    }
    FILE *in = fopen(src, "rb");
    if (in == NULL) return -1;
    FILE *out = fopen(dst, "wb");
    if (out == NULL) {
        int err = errno;
        fclose(in);
        errno = err;
        return -1;
    }
    // The buffers must live until fclose, so they are freed after it.
    char *in_buf = malloc((size_t)size);
    char *out_buf = malloc((size_t)size);
    char *chunk = malloc((size_t)size);
    long long total = -1;
    int err = 0;
    if (in_buf == NULL || out_buf == NULL || chunk == NULL) {
        err = ENOMEM;
    } else if (setvbuf(in, in_buf, _IOFBF, (size_t)size) != 0 ||
               setvbuf(out, out_buf, _IOFBF, (size_t)size) != 0) {
        err = errno != 0 ? errno : EINVAL;
    } else {
        total = 0;
        size_t n;
        while ((n = fread(chunk, 1, (size_t)size, in)) > 0) {
            if (fwrite(chunk, 1, n, out) != n) {
                err = errno;
                total = -1;
                break;
            }
            total += (long long)n;
        }
        if (total >= 0 && ferror(in)) {
            err = errno;
            total = -1;
        }
    }
    fclose(in);
    if (fclose(out) != 0 && total >= 0) {
        err = errno;
        total = -1;
    }
    free(in_buf);
    free(out_buf);
    free(chunk);
    if (total < 0) errno = err;
    return total;
}
