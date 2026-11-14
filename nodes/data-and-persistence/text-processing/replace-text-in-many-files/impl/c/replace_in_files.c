// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

// mkstemp, fchmod and fsync are in POSIX, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "replace_in_files.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Reads the whole file at path into a new buffer that the caller frees.
static int read_file(const char *path, char **data, size_t *len) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return -1;
    }
    size_t cap = 4096, n = 0;
    char *buf = malloc(cap);
    while (buf != NULL) {
        n += fread(buf + n, 1, cap - n, f);
        if (n < cap) {
            break;
        }
        char *bigger = realloc(buf, cap * 2);
        if (bigger == NULL) {
            free(buf);
            buf = NULL;
            break;
        }
        buf = bigger;
        cap *= 2;
    }
    int e = errno;
    if (buf == NULL || ferror(f)) {
        free(buf);
        fclose(f);
        errno = buf == NULL ? ENOMEM : e;
        return -1;
    }
    fclose(f);
    *data = buf;
    *len = n;
    return 0;
}

// Returns a pointer to the first instance of the m bytes of needle in the n
// bytes of haystack, or NULL.
static const char *find(const char *haystack, size_t n, const char *needle, size_t m) {
    for (size_t i = 0; m <= n && i <= n - m; i++) {
        if (memcmp(haystack + i, needle, m) == 0) {
            return haystack + i;
        }
    }
    return NULL;
}

// Writes the len bytes of data to a new file next to path, and renames it over
// path.
static int write_atomically(const char *path, const char *data, size_t len) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return -1;
    }
    // mkstemp changes the six X characters to make a name that no file has.
    // The name is path with a suffix, so it is in the same directory.
    size_t plen = strlen(path);
    char *tmp = malloc(plen + sizeof ".XXXXXX");
    if (tmp == NULL) {
        return -1;
    }
    memcpy(tmp, path, plen);
    memcpy(tmp + plen, ".XXXXXX", sizeof ".XXXXXX");
    int fd = mkstemp(tmp);
    if (fd < 0) {
        free(tmp);
        return -1;
    }
    size_t done = 0;
    while (done < len) {
        ssize_t w = write(fd, data + done, len - done);
        if (w < 0) {
            goto fail;
        }
        done += (size_t)w;
    }
    // mkstemp makes the file readable and writable only by its owner.
    if (fchmod(fd, st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)) != 0 || fsync(fd) != 0) {
        goto fail;
    }
    int closed = close(fd);
    fd = -1;
    if (closed != 0 || rename(tmp, path) != 0) {
        goto fail;
    }
    free(tmp);
    return 0;

fail:;
    int e = errno;
    if (fd >= 0) {
        close(fd);
    }
    unlink(tmp);
    free(tmp);
    errno = e;
    return -1;
}

int replace_in_files(const char *const paths[], size_t n, const char *old_text, const char *new_text,
                     size_t counts[]) {
    size_t old_len = strlen(old_text), new_len = strlen(new_text);
    if (old_len == 0) {
        errno = EINVAL;
        return -1;
    }
    for (size_t i = 0; i < n; i++) {
        char *data;
        size_t len;
        if (read_file(paths[i], &data, &len) != 0) {
            return -1;
        }
        // First count the instances, to know the size of the new data. The
        // search goes on after the end of each instance.
        size_t count = 0;
        const char *p = data, *end = data + len;
        const char *at;
        while ((at = find(p, (size_t)(end - p), old_text, old_len)) != NULL) {
            count++;
            p = at + old_len;
        }
        if (count > 0) {
            size_t out_len = len - count * old_len + count * new_len;
            char *out = malloc(out_len + 1);
            if (out == NULL) {
                free(data);
                return -1;
            }
            char *q = out;
            p = data;
            while ((at = find(p, (size_t)(end - p), old_text, old_len)) != NULL) {
                memcpy(q, p, (size_t)(at - p));
                q += at - p;
                memcpy(q, new_text, new_len);
                q += new_len;
                p = at + old_len;
            }
            memcpy(q, p, (size_t)(end - p));
            int r = write_atomically(paths[i], out, out_len);
            free(out);
            if (r != 0) {
                free(data);
                return -1;
            }
        }
        free(data);
        counts[i] = count;
    }
    return 0;
}
