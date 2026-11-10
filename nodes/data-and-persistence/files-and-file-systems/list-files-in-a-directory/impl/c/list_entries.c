// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

#define _POSIX_C_SOURCE 200809L

#include "list_entries.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct list {
    char **items;
    size_t len, cap;
};

static int push(struct list *l, const char *prefix, const char *name, const char *suffix) {
    if (l->len == l->cap) {
        size_t cap = l->cap ? 2 * l->cap : 16;
        char **items = realloc(l->items, cap * sizeof *items);
        if (items == NULL) return -1;
        l->items = items;
        l->cap = cap;
    }
    size_t size = strlen(prefix) + strlen(name) + strlen(suffix) + 1;
    char *s = malloc(size);
    if (s == NULL) return -1;
    snprintf(s, size, "%s%s%s", prefix, name, suffix);
    l->items[l->len++] = s;
    return 0;
}

// walk adds the entries of the directory path to l. prefix is the relative
// path of the directory, with a "/" at the end, or "" for the root.
static int walk(const char *path, const char *prefix, bool recursive, struct list *l) {
    DIR *d = opendir(path);
    if (d == NULL) return -1;
    int rc = 0;
    for (;;) {
        errno = 0;
        struct dirent *e = readdir(d);
        if (e == NULL) {
            if (errno != 0) rc = -1;
            break;
        }
        // readdir also returns the entries "." and "..".
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;
        // AT_SYMLINK_NOFOLLOW gives the type of a link, not of its target.
        struct stat st;
        if (fstatat(dirfd(d), e->d_name, &st, AT_SYMLINK_NOFOLLOW) != 0) {
            rc = -1;
            break;
        }
        bool is_dir = S_ISDIR(st.st_mode);
        if (push(l, prefix, e->d_name, is_dir ? "/" : "") != 0) {
            rc = -1;
            break;
        }
        if (is_dir && recursive) {
            const char *rel = l->items[l->len - 1];
            size_t size = strlen(path) + strlen(e->d_name) + 2;
            char *sub = malloc(size);
            if (sub == NULL) {
                rc = -1;
                break;
            }
            snprintf(sub, size, "%s/%s", path, e->d_name);
            rc = walk(sub, rel, recursive, l);
            free(sub);
            if (rc != 0) break;
        }
    }
    int saved = errno;
    closedir(d);
    errno = saved;
    return rc;
}

static int compare(const void *a, const void *b) {
    // strcmp compares the bytes as unsigned char. For UTF-8 that is code point order.
    return strcmp(*(char *const *)a, *(char *const *)b);
}

int list_entries(const char *root, bool recursive, char ***out, size_t *n) {
    struct list l = {0};
    if (walk(root, "", recursive, &l) != 0) {
        int saved = errno;
        free_entries(l.items, l.len);
        errno = saved;
        return -1;
    }
    if (l.len > 0) qsort(l.items, l.len, sizeof *l.items, compare);
    *out = l.items;
    *n = l.len;
    return 0;
}

void free_entries(char **entries, size_t n) {
    for (size_t i = 0; i < n; i++) free(entries[i]);
    free(entries);
}
