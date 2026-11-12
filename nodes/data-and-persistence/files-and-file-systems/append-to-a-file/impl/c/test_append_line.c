// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

// fork and waitpid are in POSIX.1-2008, not in ISO C.
#define _POSIX_C_SOURCE 200809L

#include "append_line.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static const char *path = "append_line_log.txt";

static void write_file(const char *data) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fputs(data, f) >= 0);
    assert(fclose(f) == 0);
}

static char *read_file(size_t *len) {
    FILE *f = fopen(path, "rb");
    assert(f != NULL);
    size_t cap = 1 << 20, n;
    char *buf = malloc(cap);
    assert(buf != NULL);
    n = fread(buf, 1, cap - 1, f);
    assert(!ferror(f) && n < cap - 1);
    buf[n] = '\0';
    fclose(f);
    *len = n;
    return buf;
}

// old is NULL when no file is there.
static void check(const char *name, const char *old, const char *line, const char *want) {
    remove(path);
    if (old != NULL) write_file(old);
    size_t len;
    if (append_line(path, line) != 0) {
        fprintf(stderr, "case %s: error %d\n", name, errno);
        exit(1);
    }
    char *got = read_file(&len);
    if (len != strlen(want) || memcmp(got, want, len) != 0) {
        fprintf(stderr, "case %s: wrong content\n", name);
        exit(1);
    }
    free(got);
}

static void check_error(const char *name, const char *file, const char *line, int want) {
    remove(path);
    errno = 0;
    if (append_line(file, line) != -1 || errno != want) {
        fprintf(stderr, "case %s: want errno %d, got %d\n", name, want, errno);
        exit(1);
    }
    if (access(path, F_OK) == 0) {
        fprintf(stderr, "case %s: the file was made\n", name);
        exit(1);
    }
}

enum { WRITERS = 8, LINES = 250 };

static void text(char *buf, size_t size, int w, int i) {
    snprintf(buf, size, "writer %d line %03d xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", w, i);
}

// Each writer is a separate process, as with many programs that share a log.
static void many_writers(void) {
    remove(path);
    char buf[128];
    for (int w = 0; w < WRITERS; w++) {
        pid_t pid = fork();
        assert(pid >= 0);
        if (pid == 0) {
            for (int i = 0; i < LINES; i++) {
                text(buf, sizeof buf, w, i);
                if (append_line(path, buf) != 0) _exit(1);
            }
            _exit(0);
        }
    }
    for (int w = 0; w < WRITERS; w++) {
        int status;
        assert(wait(&status) > 0);
        assert(WIFEXITED(status) && WEXITSTATUS(status) == 0);
    }
    size_t len;
    char *got = read_file(&len);
    static int seen[WRITERS][LINES];
    int count = 0;
    for (char *s = strtok(got, "\n"); s != NULL; s = strtok(NULL, "\n"), count++) {
        int w, i;
        if (sscanf(s, "writer %d line %d", &w, &i) != 2 || w < 0 || w >= WRITERS || i < 0 ||
            i >= LINES) {
            fprintf(stderr, "many writers: bad line\n");
            exit(1);
        }
        text(buf, sizeof buf, w, i);
        if (strcmp(s, buf) != 0 || seen[w][i]++ != 0) {
            fprintf(stderr, "many writers: line %d %d is wrong or twice\n", w, i);
            exit(1);
        }
    }
    if (count != WRITERS * LINES) {
        fprintf(stderr, "many writers: %d lines\n", count);
        exit(1);
    }
    free(got);
    remove(path);
}

int main(void) {
    check("new-file", NULL, "first", "first\n");
    check("append-existing", "one\n", "two", "one\ntwo\n");
    check("new-file-empty-line", NULL, "", "\n");
    check("empty-line", "a\n", "", "a\n\n");
    check("empty-file", "", "x", "x\n");
    check("no-final-newline", "one", "two", "onetwo\n");
    check("crlf-file", "one\r\n", "two", "one\r\ntwo\n");
    check("whitespace", "", "  two spaces\ttab ", "  two spaces\ttab \n");
    check("non-ascii", "na\xc3\xafve\n", "caf\xc3\xa9 \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e \xf0\x9f\x99\x82",
          "na\xc3\xafve\ncaf\xc3\xa9 \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e \xf0\x9f\x99\x82\n");
    check_error("missing-dir", "missing_dir/log.txt", "x", ENOENT);
    check_error("newline-in-line", path, "a\nb", EINVAL);
    check_error("cr-in-line", path, "a\rb", EINVAL);
    many_writers();
    remove(path);
    puts("all tests passed");
    return 0;
}
