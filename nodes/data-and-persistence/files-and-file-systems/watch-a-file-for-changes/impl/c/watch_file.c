// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

#define _POSIX_C_SOURCE 200809L
#ifdef __APPLE__
#define _DARWIN_C_SOURCE
#endif

#include "watch_file.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __APPLE__
#define MTIME(st) ((st).st_mtimespec)
#else
#define MTIME(st) ((st).st_mtim)
#endif

// Reads the signature of path into w. A missing file is not an error.
static int take_signature(const char *path, bool *exists, off_t *size, struct timespec *mtime) {
    struct stat st;
    if (stat(path, &st) != 0) {
        if (errno != ENOENT) return -1;
        *exists = false;
        *size = 0;
        mtime->tv_sec = 0;
        mtime->tv_nsec = 0;
        return 0;
    }
    if (S_ISDIR(st.st_mode)) {
        errno = EISDIR;
        return -1;
    }
    *exists = true;
    *size = st.st_size;
    *mtime = MTIME(st);
    return 0;
}

int watcher_init(struct file_watcher *w, const char *path) {
    w->path = path;
    return take_signature(path, &w->exists, &w->size, &w->mtime);
}

int watcher_poll(struct file_watcher *w, enum watch_change *change) {
    bool exists;
    off_t size;
    struct timespec mtime;
    if (take_signature(w->path, &exists, &size, &mtime) != 0) return -1;
    if (!w->exists && !exists) {
        *change = WATCH_NONE;
    } else if (!w->exists) {
        *change = WATCH_CREATED;
    } else if (!exists) {
        *change = WATCH_DELETED;
    } else if (size != w->size || mtime.tv_sec != w->mtime.tv_sec || mtime.tv_nsec != w->mtime.tv_nsec) {
        *change = WATCH_MODIFIED;
    } else {
        *change = WATCH_NONE;
    }
    w->exists = exists;
    w->size = size;
    w->mtime = mtime;
    return 0;
}

const char *watch_change_name(enum watch_change change) {
    switch (change) {
    case WATCH_CREATED: return "created";
    case WATCH_DELETED: return "deleted";
    case WATCH_MODIFIED: return "modified";
    default: return "none";
    }
}

// Writes text to path with the flags of open. Returns 0, or -1 with errno.
static int put(const char *path, const char *text, int flags) {
    int fd = open(path, flags, 0644);
    if (fd < 0) return -1;
    size_t len = strlen(text);
    while (len > 0) {
        ssize_t n = write(fd, text, len);
        if (n < 0) {
            if (errno == EINTR) continue;
            int saved = errno;
            close(fd);
            errno = saved;
            return -1;
        }
        text += n;
        len -= (size_t)n;
    }
    return close(fd);
}

// Makes the change of one step of a case (the entry of the cases uses it).
static int apply_step(const char *path, const char *step, long number) {
    const char *colon = strchr(step, ':');
    size_t klen = colon ? (size_t)(colon - step) : strlen(step);
    const char *text = colon ? colon + 1 : "";
    char tmp[4096];
    int rc;
#define IS(k) (klen == strlen(k) && strncmp(step, k, klen) == 0)
    if (IS("create") || IS("write")) {
        rc = put(path, text, O_WRONLY | O_CREAT | O_TRUNC);
    } else if (IS("append")) {
        rc = put(path, text, O_WRONLY | O_APPEND);
    } else if (IS("replace")) {
        if ((size_t)snprintf(tmp, sizeof tmp, "%s.tmp", path) >= sizeof tmp) {
            errno = ENAMETOOLONG;
            return -1;
        }
        rc = put(tmp, text, O_WRONLY | O_CREAT | O_TRUNC);
        if (rc == 0) rc = rename(tmp, path);
    } else if (IS("recreate")) {
        rc = unlink(path);
        if (rc == 0) rc = put(path, text, O_WRONLY | O_CREAT | O_TRUNC);
    } else if (IS("delete")) {
        rc = unlink(path);
    } else if (IS("mkdir")) {
        rc = mkdir(path, 0755);
    } else if (IS("touch") || IS("none")) {
        rc = 0;
    } else {
        errno = EINVAL;
        return -1;
    }
    if (rc != 0) return -1;
    struct stat st;
    if (!IS("none") && stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
        struct timespec times[2] = {{WATCH_BASE_MTIME + number, 0}, {WATCH_BASE_MTIME + number, 0}};
        if (utimensat(AT_FDCWD, path, times, 0) != 0) return -1;
    }
#undef IS
    return 0;
}

int watch_changes(const char *path, const char *const *steps, size_t n, enum watch_change *out) {
    struct file_watcher w;
    if (watcher_init(&w, path) != 0) return -1;
    for (size_t i = 0; i < n; i++) {
        if (apply_step(path, steps[i], (long)i + 1) != 0) return -1;
        if (watcher_poll(&w, &out[i]) != 0) return -1;
    }
    return 0;
}
