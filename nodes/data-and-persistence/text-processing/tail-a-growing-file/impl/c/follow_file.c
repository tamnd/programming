// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

// open, fstat, pread and nanosleep are in POSIX, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "follow_file.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

int follower_open(struct follower *f, const char *path) {
    *f = (struct follower){.fd = -1};
    struct stat st;
    f->path = malloc(strlen(path) + 1);
    if (f->path == NULL) {
        return -1;
    }
    strcpy(f->path, path);
    f->fd = open(path, O_RDONLY);
    if (f->fd < 0 || fstat(f->fd, &st) != 0) {
        int e = errno;
        follower_close(f);
        errno = e;
        return -1;
    }
    f->offset = st.st_size;
    return 0;
}

void follower_close(struct follower *f) {
    if (f->fd >= 0) {
        close(f->fd);
    }
    free(f->path);
    free(f->pending);
    *f = (struct follower){.fd = -1};
}

static int flush(struct follower *f, follow_emit emit, void *arg) {
    if (f->npending == 0) {
        return 0;
    }
    size_t len = f->npending;
    f->npending = 0;
    return emit(f->pending, len, arg);
}

// Reads the new bytes, and calls emit with each complete line.
static int read_new(struct follower *f, follow_emit emit, void *arg) {
    for (;;) {
        if (f->cap - f->npending < 65536) {
            size_t cap = f->npending + 65536;
            char *bigger = realloc(f->pending, cap);
            if (bigger == NULL) {
                return -1;
            }
            f->pending = bigger;
            f->cap = cap;
        }
        ssize_t n = pread(f->fd, f->pending + f->npending, f->cap - f->npending, f->offset);
        if (n < 0 && errno == EINTR) {
            continue;
        }
        if (n <= 0) {
            return n < 0 ? -1 : 0;
        }
        f->offset += n;
        // Split the bytes at each LF. The bytes before the new bytes have no LF.
        size_t start = 0, end = f->npending + (size_t)n;
        for (size_t i = f->npending; i < end; i++) {
            if (f->pending[i] == '\n') {
                if (emit(f->pending + start, i - start, arg) != 0) {
                    return -1;
                }
                start = i + 1;
            }
        }
        memmove(f->pending, f->pending + start, end - start);
        f->npending = end - start;
    }
}

int follower_poll(struct follower *f, follow_emit emit, void *arg) {
    if (f->fd >= 0) {
        // Look at the path before the read. Then the read gets each byte that
        // was written before a rename or a delete.
        struct stat now, open_file;
        int found = stat(f->path, &now) == 0;
        if (!found && errno != ENOENT) {
            return -1;
        }
        if (fstat(f->fd, &open_file) != 0) {
            return -1;
        }
        if (open_file.st_size < f->offset) {
            // The file is shorter than the offset, so it was truncated.
            if (flush(f, emit, arg) != 0) {
                return -1;
            }
            f->offset = 0;
        }
        if (read_new(f, emit, arg) != 0) {
            return -1;
        }
        if (!found || now.st_dev != open_file.st_dev || now.st_ino != open_file.st_ino) {
            // The file was deleted or renamed. Its rest is read.
            close(f->fd);
            f->fd = -1;
            if (flush(f, emit, arg) != 0) {
                return -1;
            }
        }
    }
    if (f->fd < 0) {
        f->fd = open(f->path, O_RDONLY);
        if (f->fd < 0) {
            return errno == ENOENT ? 0 : -1;
        }
        f->offset = 0;
        return read_new(f, emit, arg);
    }
    return 0;
}

int follower_follow(struct follower *f, unsigned interval_ms, int (*stop)(void *), follow_emit emit, void *arg) {
    struct timespec interval = {.tv_sec = interval_ms / 1000, .tv_nsec = (long)(interval_ms % 1000) * 1000000};
    for (;;) {
        if (follower_poll(f, emit, arg) != 0) {
            return -1;
        }
        if (stop(arg)) {
            return 0;
        }
        nanosleep(&interval, NULL);
    }
}

// Writes text to the file at path, which open opens with flags.
static int write_text(const char *path, int flags, const char *text) {
    int fd = open(path, O_WRONLY | flags, 0644);
    if (fd < 0) {
        return -1;
    }
    size_t len = strlen(text);
    while (len > 0) {
        ssize_t n = write(fd, text, len);
        if (n < 0 && errno == EINTR) {
            continue;
        }
        if (n < 0) {
            int e = errno;
            close(fd);
            errno = e;
            return -1;
        }
        text += n;
        len -= (size_t)n;
    }
    return close(fd);
}

// The polls of follow_file. The last string gets the lines of a poll.
struct polls {
    char **items;
    size_t n, len;
};

static int add_line(const char *line, size_t len, void *arg) {
    struct polls *p = arg;
    char *last = p->items[p->n - 1];
    char *bigger = realloc(last, p->len + len + 2);
    if (bigger == NULL) {
        return -1;
    }
    memcpy(bigger + p->len, line, len);
    bigger[p->len + len] = '\n';
    bigger[p->len + len + 1] = '\0';
    p->items[p->n - 1] = bigger;
    p->len += len + 1;
    return 0;
}

static int do_step(struct follower *f, const char *path, const char *old, const char *step, struct polls *p) {
    const char *colon = strchr(step, ':');
    size_t oplen = colon != NULL ? (size_t)(colon - step) : strlen(step);
    const char *text = colon != NULL ? colon + 1 : "";
#define OP(name) (oplen == strlen(name) && strncmp(step, name, oplen) == 0)
    if (OP("poll")) {
        char **bigger = realloc(p->items, (p->n + 1) * sizeof *bigger);
        if (bigger == NULL) {
            return -1;
        }
        p->items = bigger;
        p->items[p->n] = calloc(1, 1);
        if (p->items[p->n] == NULL) {
            return -1;
        }
        p->n++;
        p->len = 0;
        return follower_poll(f, add_line, p);
    }
    if (OP("append")) {
        return write_text(path, O_CREAT | O_APPEND, text);
    }
    if (OP("append-old")) {
        return write_text(old, O_CREAT | O_APPEND, text);
    }
    if (OP("truncate")) {
        return write_text(path, O_CREAT | O_TRUNC, text);
    }
    if (OP("rotate")) {
        return rename(path, old) != 0 ? -1 : write_text(path, O_CREAT | O_EXCL, "");
    }
    if (OP("delete")) {
        return unlink(path);
    }
    if (OP("create")) {
        return write_text(path, O_CREAT | O_EXCL, text);
    }
#undef OP
    errno = EINVAL;
    return -1;
}

int follow_file(const char *path, const char *initial, const char *const steps[], char ***polls, size_t *npolls) {
    if (write_text(path, O_CREAT | O_TRUNC, initial) != 0) {
        return -1;
    }
    struct follower f;
    if (follower_open(&f, path) != 0) {
        return -1;
    }
    char *old = malloc(strlen(path) + 3);
    struct polls p = {0};
    int r = old == NULL ? -1 : 0;
    if (old != NULL) {
        strcpy(old, path);
        strcat(old, ".1");
    }
    for (size_t i = 0; r == 0 && steps[i] != NULL; i++) {
        r = do_step(&f, path, old, steps[i], &p);
    }
    int e = errno;
    follower_close(&f);
    free(old);
    if (r != 0) {
        for (size_t i = 0; i < p.n; i++) {
            free(p.items[i]);
        }
        free(p.items);
        errno = e;
        return -1;
    }
    *polls = p.items;
    *npolls = p.n;
    return 0;
}
